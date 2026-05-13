#pragma once

#include "LoxCallable.h"
#include "Stmt.h"
#include "Interpreter.h"

class LoxFunction : public LoxCallable
{
private:
    Function* declaration;
public:
    LoxFunction(Function* declaration) : declaration(declaration) {}
    ~LoxFunction() {}

    std::any call(Interpreter* interpreter, const std::vector<std::any>& arguments) override {
        std::shared_ptr<Environment> environment = std::make_shared<Environment>(interpreter->globals);

        for (int i = 0; i < arguments.size(); i++) {
            environment->define(declaration->params.at(i)->getLexeme(), arguments.at(i));
        }
        interpreter->executeBlock(declaration->body, environment);

        return nullptr;
    }

    int arity() override {
        return declaration->params.size();
    }

    std::string toString() override {
        return "<fn" + declaration->name->getLexeme() + ">";
    }
};