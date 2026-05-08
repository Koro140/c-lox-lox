#pragma once

#include <string>
#include <any>
#include <iostream>

enum TokenType {
    TOK_COMMA, TOK_DOT, TOK_SEMICOLON, TOK_PLUS, TOK_MINUS, TOK_SLASH, TOK_STAR,
    TOK_LEFT_PAREN, TOK_RIGHT_PAREN, TOK_LEFT_BRACE, TOK_RIGHT_BRACE,

    TOK_BANG,TOK_BANG_EQUAL,
    TOK_EQUAL,TOK_EQUAL_EQUAL,
    TOK_LESS,TOK_GREATER,
    TOK_LESS_EQUAL,TOK_GREATER_EQUAL,

    TOK_IDENTIFIER, TOK_STRING, TOK_NUMBER,

    TOK_AND, TOK_CLASS, TOK_ELSE, TOK_FALSE, TOK_FUN, TOK_FOR, TOK_IF, TOK_NIL, TOK_OR,
    TOK_PRINT, TOK_RETURN, TOK_SUPER, TOK_THIS, TOK_TRUE, TOK_VAR, TOK_WHILE,

    TOK_EOF
};

class Token
{
private:
    TokenType type;
    int line;
    std::string lexeme;
    std::any value;

    std::string tokenTypeToString(TokenType type) {
        switch (type) {
            case TOK_COMMA: return "TOK_COMMA";
            case TOK_DOT: return "TOK_DOT";
            case TOK_SEMICOLON: return "TOK_SEMICOLON";
            case TOK_PLUS: return "TOK_PLUS";
            case TOK_MINUS: return "TOK_MINUS";
            case TOK_SLASH: return "TOK_SLASH";
            case TOK_STAR: return "TOK_STAR";
            case TOK_LEFT_PAREN: return "TOK_LEFT_PAREN";
            case TOK_RIGHT_PAREN: return "TOK_RIGHT_PAREN";
            case TOK_LEFT_BRACE: return "TOK_LEFT_BRACE";
            case TOK_RIGHT_BRACE: return "TOK_RIGHT_BRACE";

            case TOK_BANG: return "TOK_BANG";
            case TOK_BANG_EQUAL: return "TOK_BANG_EQUAL";
            case TOK_EQUAL: return "TOK_EQUAL";
            case TOK_EQUAL_EQUAL: return "TOK_EQUAL_EQUAL";
            case TOK_LESS: return "TOK_LESS";
            case TOK_GREATER: return "TOK_GREATER";
            case TOK_LESS_EQUAL: return "TOK_LESS_EQUAL";
            case TOK_GREATER_EQUAL: return "TOK_GREATER_EQUAL";

            case TOK_IDENTIFIER: return "TOK_IDENTIFIER";
            case TOK_STRING: return "TOK_STRING";
            case TOK_NUMBER: return "TOK_NUMBER";

            case TOK_AND: return "TOK_AND";
            case TOK_CLASS: return "TOK_CLASS";
            case TOK_ELSE: return "TOK_ELSE";
            case TOK_FALSE: return "TOK_FALSE";
            case TOK_FUN: return "TOK_FUN";
            case TOK_FOR: return "TOK_FOR";
            case TOK_IF: return "TOK_IF";
            case TOK_NIL: return "TOK_NIL";
            case TOK_OR: return "TOK_OR";
            case TOK_PRINT: return "TOK_PRINT";
            case TOK_RETURN: return "TOK_RETURN";
            case TOK_SUPER: return "TOK_SUPER";
            case TOK_THIS: return "TOK_THIS";
            case TOK_TRUE: return "TOK_TRUE";
            case TOK_VAR: return "TOK_VAR";
            case TOK_WHILE: return "TOK_WHILE";

            case TOK_EOF: return "TOK_EOF";
            default: return "UNKNOWN";
        }
    }
public:
    Token(TokenType type, int line, const std::string& lexeme, std::any value) {
        this->type = type;
        this->line = line;
        this->lexeme = lexeme;
        this->value = value;
    }

    std::string stringfy() {
        return std::string(tokenTypeToString(type) + " " + std::to_string(line) + " " + lexeme);
    }

    std::string getLexeme() {
        return lexeme;
    }

    std::any getValue() {
        return value;
    }

    TokenType getType() {
        return type;
    }

    int getLine() {
        return line;
    }
};