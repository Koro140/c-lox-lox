#pragma once

#include <map>
#include <string>
#include <any>

#include "Token.h"

class Environment
{
private:
    std::map<std::string, std::any> values;
    Environment* enclosing = nullptr;
public:
    Environment() {}
    Environment(Environment* enclosing) : enclosing(enclosing) {}
    ~Environment() {}

    void define(std::string name, std::any value);
    std::any get(Token* name);
    void assign(Token* name, std::any value);
};