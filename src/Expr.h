#pragma once

#include <vector>
#include <string>

#include <any>

#include "Token.h"

class Grouping;
class Binary;
class Unary;
class Literal;

class Visitor {
public:
	virtual void visit(Grouping& v) = 0;
	virtual void visit(Binary& v) = 0;
	virtual void visit(Unary& v) = 0;
	virtual void visit(Literal& v) = 0;
};

class Expr {
public:
	virtual void accept(Visitor& v) = 0;
	~Expr() = default;
};

class Grouping : public Expr {
public:
	Expr* expr;
public:
	Grouping(Expr* expr) {
		this->expr = expr;
	}
	void accept(Visitor& v) override {
		v.visit(*this);
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
	void accept(Visitor& v) override {
		v.visit(*this);
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
	void accept(Visitor& v) override {
		v.visit(*this);
	}
;};

class Literal : public Expr {
public:
	std::any value;
public:
	Literal(std::any value) {
		this->value = value;
	}
	void accept(Visitor& v) override {
		v.visit(*this);
	}
;};

