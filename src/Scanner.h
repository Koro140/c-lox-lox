#pragma once

#include <vector>
#include <map>
#include <unordered_map>

#include "Token.h"

class Scanner
{
private:
    int line = 1;
    int current = 0;
    int start = 0;

    const std::string& source;
    std::vector<Token> tokens;

    static const std::unordered_map<std::string, TokenType> keywords;

public:
    Scanner(const std::string& source) : source(source) {}
    std::vector<Token> scanTokens();

private:
    char advance();
    bool match(char c);
    char peek();
    char peekNext();

    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);

    bool isAtEnd();

    void scanToken();
    void addToken(TokenType type);
    void addToken(TokenType type, std::any value);

    void number();
    void string();
    void identifier();
};