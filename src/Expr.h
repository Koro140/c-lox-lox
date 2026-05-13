#pragma once

#include <vector>
#include <string>

#include <any>

#include "Token.h"

class Grouping;
class Assign;
class Binary;
class Call;
class Logical;
class Unary;
class Literal;
class Variable;

class ExprVisitor {
public:
	virtual std::any visit(Grouping& v) = 0;
	virtual std::any visit(Assign& v) = 0;
	virtual std::any visit(Binary& v) = 0;
	virtual std::any visit(Call& v) = 0;
	virtual std::any visit(Logical& v) = 0;
	virtual std::any visit(Unary& v) = 0;
	virtual std::any visit(Literal& v) = 0;
	virtual std::any visit(Variable& v) = 0;
};

class Expr {
public:
	virtual std::any accept(ExprVisitor& v) = 0;
	~Expr() = default;
};

class Grouping : public Expr {
public:
	Expr* expr;
public:
	Grouping(Expr* expr) {
		this->expr = expr;
	}
	std::any accept(ExprVisitor& v) override {
		return v.visit(*this);
	}
;};

class Assign : public Expr {
public:
	Token* name;
	Expr* value;
public:
	Assign(Token* name, Expr* value) {
		this->name = name;
		this->value = value;
	}
	std::any accept(ExprVisitor& v) override {
		return v.visit(*this);
	}
;};

class Binary : public Expr {
public:
	Expr* left;
	Token* op;
	Expr* right;
public:
	Binary(Expr* left, Token* op, Expr* right) {
		this->left = left;
		this->op = op;
		this->right = right;
	}
	std::any accept(ExprVisitor& v) override {
		return v.visit(*this);
	}
;};

class Call : public Expr {
public:
	Expr* calle;
	Token* paren;
	std::vector<Expr*> arguments;
public:
	Call(Expr* calle, Token* paren, std::vector<Expr*> arguments) {
		this->calle = calle;
		this->paren = paren;
		this->arguments = arguments;
	}
	std::any accept(ExprVisitor& v) override {
		return v.visit(*this);
	}
;};

class Logical : public Expr {
public:
	Expr* left;
	Token* op;
	Expr* right;
public:
	Logical(Expr* left, Token* op, Expr* right) {
		this->left = left;
		this->op = op;
		this->right = right;
	}
	std::any accept(ExprVisitor& v) override {
		return v.visit(*this);
	}
;};

class Unary : public Expr {
public:
	Token* op;
	Expr* right;
public:
	Unary(Token* op, Expr* right) {
		this->op = op;
		this->right = right;
	}
	std::any accept(ExprVisitor& v) override {
		return v.visit(*this);
	}
;};

class Literal : public Expr {
public:
	std::any value;
public:
	Literal(std::any value) {
		this->value = value;
	}
	std::any accept(ExprVisitor& v) override {
		return v.visit(*this);
	}
;};

class Variable : public Expr {
public:
	Token* name;
public:
	Variable(Token* name) {
		this->name = name;
	}
	std::any accept(ExprVisitor& v) override {
		return v.visit(*this);
	}
;};

