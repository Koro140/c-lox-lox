#include "Scanner.h"

#include "Application.h"

const std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"and", TOK_AND},
    {"class", TOK_CLASS},
    {"else", TOK_ELSE},
    {"false", TOK_FALSE},
    {"for", TOK_FOR},
    {"fun", TOK_FUN},
    {"if", TOK_IF},
    {"nil", TOK_NIL},
    {"or", TOK_OR},
    {"print", TOK_PRINT},
    {"return", TOK_RETURN},
    {"super", TOK_SUPER},
    {"this", TOK_THIS},
    {"true", TOK_TRUE},
    {"var", TOK_VAR},
    {"while", TOK_WHILE}
};

char Scanner::advance()
{
    return source[current++];
}

bool Scanner::match(char c)
{
    if (isAtEnd()) {
        return false;
    }
    if (c != source[current]) {
        return false;
    }
    
    current++;
    return true;
}

char Scanner::peek()
{
    if (isAtEnd())
    {
        return '\0';
    }
    
    return source[current];
}

char Scanner::peekNext()
{
    if (current + 1 >= source.length())
    {
        return '\0';
    }
    
    return source[current + 1];
}

bool Scanner::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

bool Scanner::isAtEnd()
{
    return current >= source.length();
}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.push_back(Token(TOK_EOF, line, "", std::any{}));

    return tokens;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(TOK_LEFT_PAREN); break;
        case ')': addToken(TOK_RIGHT_PAREN); break;
        case '{': addToken(TOK_LEFT_BRACE); break;
        case '}': addToken(TOK_RIGHT_BRACE); break;
        case ',': addToken(TOK_COMMA); break;
        case '.': addToken(TOK_DOT); break;
        case '-': addToken(TOK_MINUS); break;
        case '+': addToken(TOK_PLUS); break;
        case ';': addToken(TOK_SEMICOLON); break;
        case '*': addToken(TOK_STAR); break;
        
        case '/':
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) {
                    advance();
                }
            } else {
                addToken(TOK_SLASH);
            }
            break;

        case '=': addToken(match('=') ? TOK_EQUAL_EQUAL : TOK_EQUAL); break;
        case '!': addToken(match('=') ? TOK_BANG_EQUAL : TOK_BANG); break;
        case '<': addToken(match('=') ? TOK_LESS_EQUAL: TOK_LESS); break;
        case '>': addToken(match('=') ? TOK_GREATER_EQUAL : TOK_GREATER); break;

        case ' ':
        case '\t':
        case '\r':
            break;
        case '\n':
            line++;
            break;

        case '"':
            this->string();
            break;

        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                Application::error(line, "Unexpected character");
            }
            
    }
}

void Scanner::addToken(TokenType type) {
    addToken(type, std::any{});
}
        
void Scanner::addToken(TokenType type, std::any value) {
    tokens.push_back(Token(type, line, source.substr(start, current - start), value));
}

void Scanner::number()
{
    while (isDigit(peek())) {
        advance();
    }
    
    if (peek() == '.' && isDigit(peekNext())) {
        
        advance();

        while (isDigit(peek())) {
            advance();
        }
    }
    
    double value = std::stod(source.substr(start, current - start));
    addToken(TOK_NUMBER, value);
}

void Scanner::string()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n') line++;
        advance();
    }
    
    if (isAtEnd())
    {
        Application::error(line, "Unterminated string");
        return;
    }
    
    advance();

    addToken(TOK_STRING, source.substr(start + 1, current - start - 2));
}

void Scanner::identifier()
{
    while (isAlphaNumeric(peek()))
    {
        advance();
    }

    auto it = keywords.find(source.substr(start, current - start));

    TokenType type = TOK_IDENTIFIER; 
    
    if (it != keywords.end()) {
        type = it->second;
    } else {
        type = TOK_IDENTIFIER;
    }

    addToken(type);
}