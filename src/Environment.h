#pragma once

#include <map>
#include <string>
#include <any>
#include <memory>

#include "Token.h"

class Environment
{
private:
    std::map<std::string, std::any> values;
    std::shared_ptr<Environment> enclosing = nullptr;
public:
    Environment() {}
    Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}
    ~Environment() {
        // std::cout << "Env destroyed\n";
    }

    void define(std::string name, std::any value);
    std::any get(Token* name);
    void assign(Token* name, std::any value);
};