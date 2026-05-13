#pragma once

#include <vector>
#include <any>

class Interpreter;

class LoxCallable
{
private:

public:
    virtual ~LoxCallable() = default;

    virtual std::any call(Interpreter* interpreter, const std::vector<std::any>& arguments) = 0;
    virtual int arity() = 0;
    virtual std::string toString() = 0;
};