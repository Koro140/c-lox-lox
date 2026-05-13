#pragma once

#include "LoxCallable.h"
#include "Stmt.h"
#include "Interpreter.h"
#include "ReturnThrow.h"

class LoxFunction : public LoxCallable
{
private:
    Function* declaration;
    std::shared_ptr<Environment> closure;
public:
    LoxFunction(Function* declaration, std::shared_ptr<Environment> closure) : declaration(declaration), closure(closure) {}
    ~LoxFunction() {}

    std::any call(Interpreter* interpreter, const std::vector<std::any>& arguments) override {
        std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);

        for (int i = 0; i < arguments.size(); i++) {
            environment->define(declaration->params.at(i)->getLexeme(), arguments.at(i));
        }
        try
        {
            interpreter->executeBlock(declaration->body, environment);
        }
        catch(const ReturnThrow& e)
        {
            return e.value;
        }
        
        return nullptr;
    }

    int arity() override {
        return declaration->params.size();
    }

    std::string toString() override {
        return "<fn" + declaration->name->getLexeme() + ">";
    }
};