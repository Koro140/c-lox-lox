#include "Environment.h"

#include "Interpreter.h"

void Environment::define(std::string name, std::any value) {
    values[name] = value;
}

std::any Environment::get(Token* name) {
    if (values.find(name->getLexeme()) != values.end()) {
        return values[name->getLexeme()];
    }
    
    throw Interpreter::RuntimeError{name, "Undefined variable '" + name->getLexeme() + "' "};
}

void Environment::assign(Token* name, std::any value)
{
    if (values.find(name->getLexeme()) != values.end()) {
      values[name->getLexeme()] = value;
      return;
    }

    throw new Interpreter::RuntimeError(name, "Undefined variable '" + name->getLexeme() + "'.");
}