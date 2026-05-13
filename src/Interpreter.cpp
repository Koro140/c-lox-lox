#include "Interpreter.h"

#include "Application.h"
#include "LoxFunction.h"

void Interpreter::interpret(std::vector<Stmt*> statements)
{
    try {
        for (Stmt* statment : statements) {
            execute(statment);
        }
    }
    catch (RuntimeError error) {
        Application::runtimeError(error);
    }
}

void Interpreter::execute(Stmt *statement)
{
    statement->accept(*this);
}

void Interpreter::executeBlock(const std::vector<Stmt *> &statements, std::shared_ptr<Environment> enclosing) {
    std::shared_ptr<Environment> previous = this->environment;
    this->environment = enclosing;
    try {
        for (Stmt* statement : statements) {
            execute(statement);
        }
    } catch (...) {
        this->environment = previous;
        throw;
    }
    
    this->environment = previous;
}

std::any Interpreter::evaluate(Expr *expression)
{
    return expression->accept(*this);
}

std::any Interpreter::visit(Grouping& v) {
    return evaluate(v.expr);
}

std::any Interpreter::visit(Binary& v) {
    std::any left = evaluate(v.left);
    std::any right = evaluate(v.right);

    switch (v.op->getType())
    {
    case TOK_MINUS:
        checkNumberOperands(*v.op, left, right);
        return std::any_cast<double>(left) - std::any_cast<double>(right); 
    case TOK_SLASH:
        checkNumberOperands(*v.op, left, right);
        return std::any_cast<double>(left) / std::any_cast<double>(right); 
    case TOK_STAR:
        checkNumberOperands(*v.op, left, right);
        return std::any_cast<double>(left) * std::any_cast<double>(right);
    case TOK_PLUS:
        if (left.type() == typeid(double) && right.type() == typeid(double)) {
            return std::any_cast<double>(left) + std::any_cast<double>(right);
        } 
        if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
            return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
        }
        break;
    case TOK_LESS:
        checkNumberOperands(*v.op, left, right);
        return std::any_cast<double>(left) < std::any_cast<double>(right); 
    case TOK_LESS_EQUAL:
        checkNumberOperands(*v.op, left, right);
        return std::any_cast<double>(left) <= std::any_cast<double>(right); 
    case TOK_GREATER:
        checkNumberOperands(*v.op, left, right);
        return std::any_cast<double>(left) > std::any_cast<double>(right); 
    case TOK_GREATER_EQUAL:
        checkNumberOperands(*v.op, left, right);
        return std::any_cast<double>(left) >= std::any_cast<double>(right); 

    
    case TOK_BANG_EQUAL:
        return !isEqual(left, right);
    case TOK_EQUAL_EQUAL:
        return isEqual(left, right);
        
        
    default:
        break;
    }
}

std::any Interpreter::visit(Call &v)
{
    std::any callee = evaluate(v.calle);
    std::vector<std::any> arguments;
    for (Expr* argument : v.arguments)
    {
        arguments.push_back(evaluate(argument));
    }
    
    if (callee.type() != typeid(std::shared_ptr<LoxCallable>)) {
        throw RuntimeError{v.paren,"Can only call functions and classes."};
    }
    
    auto function = std::any_cast<std::shared_ptr<LoxCallable>>(callee);

    if (arguments.size() != function->arity()) {
      throw RuntimeError(v.paren, "Expected " +
            std::to_string(function->arity()) + " arguments but got " +
            std::to_string(arguments.size()) + ".");
    }
    
    return function->call(this, arguments);
}

std::any Interpreter::visit(Logical &v)
{
    std::any left = evaluate(v.left);
    if (v.op->getType() == TOK_OR) {
        if (isTruthy(left)) {
            return left;
        }
    } else {
        if (!isTruthy(left)) {
            return left;
        }
    }
    
    return evaluate(v.right);
}

std::any Interpreter::visit(Assign &v) {
    std::any value = evaluate(v.value);
    environment->assign(v.name, value);

    return value;
}

std::any Interpreter::visit(Unary& v) {
    std::any right = evaluate(v.right);

    switch (v.op->getType())
    {
    case TOK_MINUS:
        checkNumberOperand(*v.op, right);
        return -std::any_cast<double>(right);
    case TOK_BANG:
        return !isTruthy(right);
    default:
        break;
    }
    return right;
}

std::any Interpreter::visit(Literal& v) {
    return v.value;
}

std::any Interpreter::visit(Variable &v)
{
    return environment->get(v.name);
}

std::any Interpreter::visit(Print &v)
{
    std::any value = evaluate(v.expr);
    std::cout << stringfy(value) << std::endl;

    return nullptr;
}

std::any Interpreter::visit(Return &v)
{
    std::any value = nullptr;
    if (v.value != nullptr)
    {
        value = evaluate(v.value);
    }
    
    throw ReturnThrow{value};
}

std::any Interpreter::visit(While &v)
{
    while(isTruthy(evaluate(v.condition))) {
        execute(v.body);
    }

    return nullptr;
}

std::any Interpreter::visit(Var &v)
{
    std::any value = nullptr;
    if (v.right != nullptr) {
        value = evaluate(v.right);
    }

    environment->define(v.name->getLexeme(), value);

    return nullptr;
}

std::any Interpreter::visit(If &v)
{
    if (isTruthy(evaluate(v.condition))) {
        execute(v.thenBranch);
    } else {
        if (v.elseBranch != nullptr) {
            execute(v.elseBranch);
        }
    }

    return nullptr;
}

std::any Interpreter::visit(Block &v)
{
    executeBlock(v.statements, std::make_shared<Environment>(this->environment));

    return nullptr;
}

std::any Interpreter::visit(Expression &v)
{
    return evaluate(v.expr);
}

std::any Interpreter::visit(Function &v)
{
    std::shared_ptr<LoxCallable> function = std::make_shared<LoxFunction>(&v, environment);
    environment->define(v.name->getLexeme(), function);

    return nullptr;
}

bool Interpreter::isTruthy(const std::any& value)
{
    if (value.type() == typeid(nullptr)) {
        return false;
    }
    if (value.type() == typeid(bool))
    {
        return std::any_cast<bool>(value);
    }
    
    return true;
}

bool Interpreter::isEqual(const std::any &left, const std::any &right)
{
    if (left.type() == typeid(nullptr) && right.type() == typeid(nullptr)) return true;

    if (left.type() == typeid(double) && right.type() == typeid(double)) {
        return std::any_cast<double>(left) == std::any_cast<double>(right); 
    }
    
    if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
        return std::any_cast<std::string>(left) == std::any_cast<std::string>(right); 
    }

    // Implicit return of false
    return false;
}

void Interpreter::checkNumberOperand(Token &tok, const std::any &value)
{
    if (value.type() == typeid(double)) return;
    else {
        throw RuntimeError{&tok, "Operand must be a number"};
    }
}

void Interpreter::checkNumberOperands(Token &tok, const std::any &a, const std::any &b)
{
    if (a.type() == typeid(double) && b.type() == typeid(double)) return;
    else {
        throw RuntimeError{&tok, "Operands must be two numbers or two strings."};
    }
}