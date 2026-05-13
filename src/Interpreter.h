#pragma once
#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"
#include "LoxCallable.h"

#include <string>
#include <memory>
#include <ctime>
#include <cmath>
class Interpreter : public ExprVisitor, public StmtVisitor
{
public:
    std::shared_ptr<Environment> globals;
private:
    std::any result;
    std::shared_ptr<Environment> environment;
public:
    
    Interpreter() {
        globals = std::make_shared<Environment>();
        environment = globals;

        class ClockCallable : public LoxCallable
        {
        public:
            std::any call(Interpreter* interpreter, const std::vector<std::any>& arguments) override {
                return (double)std::time(nullptr);
            }
            int arity() override {
                return 0;
            }

            std::string toString() override {
                return "<native fn>";
            }
        };
        std::shared_ptr<LoxCallable> cfn = std::make_shared<ClockCallable>();
        globals->define("clock", cfn);
    }

    void interpret(std::vector<Stmt*> statements);
    class RuntimeError : public std::runtime_error
    {
        private:
        Token* tok;
        public:
        RuntimeError(Token* tok, const std::string& message) : std::runtime_error(message) {
            this->tok = tok;
        }
        
        Token* getToken() {return tok;}
    };
    
    void executeBlock(const std::vector<Stmt*>& statements, std::shared_ptr<Environment> enclosing);
private:
    void execute(Stmt* statement);
    std::any evaluate(Expr* expression);

	std::any visit(Grouping& v) override;
	std::any visit(Binary& v) override;
	std::any visit(Call& v) override;
    std::any visit(Logical& v) override;
	std::any visit(Assign& v) override;
	std::any visit(Unary& v) override;
	std::any visit(Literal& v) override;
    std::any visit(Variable& v) override;

    std::any visit(Print& v) override;
    std::any visit(Return& v) override;
    std::any visit(While& v) override;
    std::any visit(Var& v) override;
    std::any visit(If& v) override;
    std::any visit(Block& v) override;
    std::any visit(Expression& v) override;
    std::any visit(Function& v) override;

    bool isTruthy(const std::any& value);
    bool isEqual(const std::any& left, const std::any& right);
    void checkNumberOperand(Token& tok, const std::any& value);
    void checkNumberOperands(Token& tok, const std::any& a,const std::any& b);

    std::string stringfy(std::any value) {
        if (value.type() == typeid(nullptr)) {
            return "nil";
        }
        if (value.type() == typeid(double)) {
            double d = std::any_cast<double>(value);
            // Check if it's a whole number
            if (d == std::floor(d)) {
                return std::to_string((long long)d);
            }
            return std::to_string(d);
        }
        
        if (value.type() == typeid(bool)) {
            return isTruthy(value) ? "true" : "false";
        }

        return std::any_cast<std::string>(value);
    }
};