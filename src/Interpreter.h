#pragma once
#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"

#include <string>
#include <memory>

class Interpreter : public ExprVisitor, public StmtVisitor
{
private:
    std::any result;
    std::shared_ptr<Environment> environment;
public:
    
    Interpreter() {
        environment = std::make_shared<Environment>();
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
private:
    void execute(Stmt* statement);
    void executeBlock(const std::vector<Stmt*>& statements, std::shared_ptr<Environment> enclosing);
    std::any evaluate(Expr* expression);

	void visit(Grouping& v) override;
	void visit(Binary& v) override;
    void visit(Logical& v) override;
	void visit(Assign& v) override;
	void visit(Unary& v) override;
	void visit(Literal& v) override;
    void visit(Variable& v) override;

    void visit(Print& v) override;
    void visit(While& v) override;
    void visit(Var& v) override;
    void visit(If& v) override;
    void visit(Block& v) override;
    void visit(Expression& v) override;

    bool isTruthy(const std::any& value);
    bool isEqual(const std::any& left, const std::any& right);
    void checkNumberOperand(Token& tok, const std::any& value);
    void checkNumberOperands(Token& tok, const std::any& a,const std::any& b);

    std::string stringfy(std::any value) {
        if (value.type() == typeid(nullptr)) {
            return "nil";
        }
        if (value.type() == typeid(double)) {
            std::string text = std::to_string(std::any_cast<double>(value));
            if (text.ends_with(".0")) {
                text = text.substr(0, text.length() - 2);
            }
            return text;
        }
        
        if (value.type() == typeid(bool)) {
            return isTruthy(value) ? "true" : "false";
        }

        return std::any_cast<std::string>(value);
    }
};