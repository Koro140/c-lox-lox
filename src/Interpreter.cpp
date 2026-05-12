#include "Interpreter.h"

#include "Application.h"

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

void Interpreter::executeBlock(const std::vector<Stmt *> &statements, std::unique_ptr<Environment> enclosing) {
    std::unique_ptr<Environment> previous = std::move(this->environment);
    this->environment = std::move(enclosing);
    
    for (Stmt* statement : statements) {
        execute(statement);
    }

    this->environment = std::move(previous);
}

std::any Interpreter::evaluate(Expr *expression)
{
    expression->accept(*this);
    return result;
}

void Interpreter::visit(Grouping& v) {
    result = evaluate(v.expr);
}

void Interpreter::visit(Binary& v) {
    std::any left = evaluate(v.left);
    std::any right = evaluate(v.right);

    switch (v.op->getType())
    {
    case TOK_MINUS:
        checkNumberOperands(*v.op, left, right);
        result = std::any_cast<double>(left) - std::any_cast<double>(right); 
        return;
    case TOK_SLASH:
        checkNumberOperands(*v.op, left, right);
        result = std::any_cast<double>(left) / std::any_cast<double>(right); 
        return;
    case TOK_STAR:
        checkNumberOperands(*v.op, left, right);
        result = std::any_cast<double>(left) * std::any_cast<double>(right); 
        return;
    case TOK_PLUS:
        if (left.type() == typeid(double) && right.type() == typeid(double)) {
            result = std::any_cast<double>(left) + std::any_cast<double>(right);
            return;
        } 
        if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
            result = std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            return;
        }
        break;
    case TOK_LESS:
        checkNumberOperands(*v.op, left, right);
        result = std::any_cast<double>(left) < std::any_cast<double>(right); 
        return;
    case TOK_LESS_EQUAL:
        checkNumberOperands(*v.op, left, right);
        result = std::any_cast<double>(left) <= std::any_cast<double>(right); 
        return;
    case TOK_GREATER:
        checkNumberOperands(*v.op, left, right);
        result = std::any_cast<double>(left) > std::any_cast<double>(right); 
        return;
    case TOK_GREATER_EQUAL:
        checkNumberOperands(*v.op, left, right);
        result = std::any_cast<double>(left) >= std::any_cast<double>(right); 
        return;

    
    case TOK_BANG_EQUAL:
        result = !isEqual(left, right);
        return;
    case TOK_EQUAL_EQUAL:
        result = isEqual(left, right);
        return;
        
    default:
        break;
    }
}

void Interpreter::visit(Logical &v)
{
    std::any left = evaluate(v.left);
    if (v.op->getType() == TOK_OR) {
        if (isTruthy(left)) {
            result = left;
            return;
        }
    } else {
        if (!isTruthy(left)) {
            result = left;
            return;
        }
    }
    
    result = evaluate(v.right);
    return;
}

void Interpreter::visit(Assign &v) {
    std::any value = evaluate(v.value);
    environment->assign(v.name, value);
}

void Interpreter::visit(Unary& v) {
    std::any right = evaluate(v.right);

    switch (v.op->getType())
    {
    case TOK_MINUS:
        checkNumberOperand(*v.op, right);
        result = -std::any_cast<double>(right);
        return;
    case TOK_BANG:
        result = !isTruthy(result);

        return;
    default:
        break;
    }
}

void Interpreter::visit(Literal& v) {
    result = v.value;
}

void Interpreter::visit(Variable &v)
{
    result = environment->get(v.name);
}

void Interpreter::visit(Print &v)
{
    std::any value = evaluate(v.expr);
    std::cout << stringfy(value) << std::endl;
}

void Interpreter::visit(While &v)
{
    while(isTruthy(evaluate(v.condition))) {
        execute(v.body);
    }
}

void Interpreter::visit(Var &v)
{
    std::any value = nullptr;
    if (v.right != nullptr) {
        value = evaluate(v.right);
    }

    environment->define(v.name->getLexeme(), value);
}

void Interpreter::visit(If &v)
{
    if (isTruthy(evaluate(v.condition))) {
        execute(v.thenBranch);
    } else {
        if (v.elseBranch != nullptr) {
            execute(v.elseBranch);
        }
    }
}

void Interpreter::visit(Block &v)
{
    executeBlock(v.statements, std::make_unique<Environment>(this->environment.get()));
}

void Interpreter::visit(Expression &v)
{
    evaluate(v.expr);
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