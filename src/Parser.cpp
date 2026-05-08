// .cpp file to resolve the circular dependancy
#include "Parser.h"
#include "Application.h"

Expr* Parser::parse() {
    try {
        return expression();
    } catch (ParseError error) {
        return nullptr;
    }
}

Expr* Parser::expression() {
        return equality();
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
    if (match({TOK_LEFT_PAREN})) {
        Expr* expr = expression();
        consume(TOK_RIGHT_PAREN, "Expect ')' after expression.");
        return new Grouping{expr};
    }
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