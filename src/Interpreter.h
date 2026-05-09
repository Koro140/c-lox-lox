#pragma once
#include "Expr.h"

#include <string>

class Interpreter : public Visitor
{
private:
    std::any result;
public:
    Interpreter() {}
    void interpret(Expr* expression);
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
    std::any evaluate(Expr* expression);

	void visit(Grouping& v) override;
	void visit(Binary& v) override;
	void visit(Unary& v) override;
	void visit(Literal& v) override;


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
        
        return std::any_cast<std::string>(value);
    }
};