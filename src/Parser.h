#pragma once

#include "Expr.h"
#include "Token.h"

#include <vector>

class Application;

class Parser
{
private:
    std::vector<Token>& tokens;
    int current = 0;
    
    class ParseError : public std::runtime_error {
        public : ParseError(const std::string& message) : std::runtime_error(message) {}
    };
public:
    Parser(std::vector<Token>& tokens) : tokens(tokens) {}
    Expr* parse();
private:
    Expr* expression();
    
    Expr* equality();
    Expr* comparison();
    Expr* term();
    Expr* factor();

    Expr* unary();

    Expr* primary();

    ParseError error(Token* token, const std::string& message);
    Token* consume(TokenType type, std::string message);

    bool match(std::vector<TokenType> types);
    bool check(TokenType type);
    Token* advance();
    Token* previous();
    bool isAtEnd();
    Token* peek();
    
    void synchronize();
};