// .cpp file to resolve the circular dependancy
#include "Parser.h"
#include "Application.h"

std::vector<Stmt*> Parser::parse() {
    std::vector<Stmt*> v;
    while (!isAtEnd()) {
        v.push_back(declaration());
    }
    
    return v;
}

Stmt* Parser::declaration() {
    try {   
        if (match({TOK_VAR})) {
            return varDeclaration();
        }
        return statement();
    } catch(const ParseError& e) {
        std::cerr << e.what() << '\n';
        synchronize();
        return nullptr;
    }
    
}

Stmt* Parser::statement() {
    if (match({TOK_PRINT})) {
        return printStatement();
    }
    if (match({TOK_LEFT_BRACE})) {
        return new Block{block()};
    }
    
    return expressionStatement();
}

Stmt *Parser::varDeclaration()
{
    Token* name = consume(TOK_IDENTIFIER, "Expect variable name");

    Expr* initializer = nullptr;
    if (match({TOK_EQUAL})) {
        initializer = expression();
    }
    consume(TOK_SEMICOLON, "Expect ';' after variable declaration");
    return new Var{name, initializer};
}

Stmt *Parser::printStatement()
{
    Expr* value = expression();
    consume(TOK_SEMICOLON, "Expect ';' after value.");
    return new Print{value};
}

std::vector<Stmt *> Parser::block()
{
    std::vector<Stmt*> statements;

    while (!check({TOK_RIGHT_BRACE}) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    
    consume(TOK_RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

Stmt* Parser::expressionStatement() {
    Expr* expr = expression();
    consume(TOK_SEMICOLON, "Expect ';' after expression.");
    return new Expression{expr};
}

Expr* Parser::expression() {
    return assignment();
}

Expr *Parser::assignment() {
    Expr* expr = equality();
    if (match({TOK_EQUAL})) {
        Token* equals = previous();
        Expr* value = assignment();

        if (typeid(*expr) == typeid(Variable)) {
            Token* name = ((Variable*)expr)->name;
            return new Assign{name, value};
        }
        
        error(equals, "Invalid assign target");
    }
    
    return expr;
}

Expr* Parser::equality() {
    Expr* expr = comparison();
    while (match({TOK_EQUAL_EQUAL, TOK_BANG_EQUAL})) {
        Token* op = previous();
        Expr* right = comparison();
        expr = new Binary{expr, op, right};
    }
    
    return expr;
}

Expr* Parser::comparison() {
    Expr* expr = term();
    while (match({TOK_LESS, TOK_LESS_EQUAL, TOK_GREATER, TOK_GREATER_EQUAL})) {
        Token* op = previous();
        Expr* right = term();
        expr = new Binary{expr, op, right};
    }
    
    return expr;
}

Expr* Parser::term() {
    Expr* expr = factor();
    while (match({TOK_PLUS, TOK_MINUS})) {
        Token* op = previous();
        Expr* right = factor();
        expr = new Binary{expr, op, right};
    }
    
    return expr;
}

Expr* Parser::factor() {
    Expr* expr = unary();
    while (match({TOK_SLASH, TOK_STAR})) {
        Token* op = previous();
        Expr* right = unary();
        expr = new Binary{expr, op, right};
    }
    
    return expr;    
}

Expr* Parser::unary() {
    if (match({TOK_BANG, TOK_MINUS}))
    {
        Token* op = previous();
        Expr* right = unary();
        return new Unary{op, right};
    }
    
    return primary();
}

Expr* Parser::primary() {
    if (match({TOK_TRUE})) return new Literal{true};
    if (match({TOK_FALSE})) return new Literal{false};
    if (match({TOK_NIL})) return new Literal{nullptr};
    if (match({TOK_NUMBER, TOK_STRING})) {
        return new Literal{previous()->getValue()};
    }


    if (match({TOK_IDENTIFIER})) {
        return new Variable{previous()};
    }

    if (match({TOK_LEFT_PAREN})) {
        Expr* expr = expression();
        consume(TOK_RIGHT_PAREN, "Expect ')' after expression.");
        return new Grouping{expr};
    }

    // TODO : fix this
    return nullptr;
}

Parser::ParseError Parser::error(Token* token, const std::string& message) {
    Application::error(token, message);
    return {"Runtime Error"};
}

Token* Parser::consume(TokenType type, std::string message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

bool Parser::match(std::vector<TokenType> types) {
    for (TokenType& type : types) {
        TokenType tokType = tokens[current].getType();
        if (type == tokType) {
            current++;
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) {
        return false;
    }
    
    return tokens[current].getType() == type;
}

Token* Parser::advance() {
    if (isAtEnd()) {
        return &tokens.back();
    }
    
    return &tokens.at(current++);
}

Token* Parser::previous() {
    return &tokens.at(current - 1);
}

bool Parser::isAtEnd() {
    return tokens.at(current).getType() == TOK_EOF; 
}

Token* Parser::peek() {
    return &tokens.at(current);
}

void Parser::synchronize() {
    advance();
    while (!isAtEnd()) {
      if (previous()->getType() == TOK_SEMICOLON) return;
      switch (peek()->getType()) {
        case TOK_CLASS:
        case TOK_FUN:
        case TOK_VAR:
        case TOK_FOR:
        case TOK_IF:
        case TOK_WHILE:
        case TOK_PRINT:
        case TOK_RETURN:
          return;
      }
      advance();
    }
}