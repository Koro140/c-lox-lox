#pragma once

#include <vector>
#include <string>

#include <any>

#include "Token.h"

class Expression;
class Print;
class While;
class Block;
class If;
class Var;

class StmtVisitor {
public:
	virtual void visit(Expression& v) = 0;
	virtual void visit(Print& v) = 0;
	virtual void visit(While& v) = 0;
	virtual void visit(Block& v) = 0;
	virtual void visit(If& v) = 0;
	virtual void visit(Var& v) = 0;
};

class Stmt {
public:
	virtual void accept(StmtVisitor& v) = 0;
	~Stmt() = default;
};

class Expression : public Stmt {
public:
	Expr* expr;
public:
	Expression(Expr* expr) {
		this->expr = expr;
	}
	void accept(StmtVisitor& v) override {
		v.visit(*this);
	}
;};

class Print : public Stmt {
public:
	Expr* expr;
public:
	Print(Expr* expr) {
		this->expr = expr;
	}
	void accept(StmtVisitor& v) override {
		v.visit(*this);
	}
;};

class While : public Stmt {
public:
	Expr* condition;
	Stmt* body;
public:
	While(Expr* condition, Stmt* body) {
		this->condition = condition;
		this->body = body;
	}
	void accept(StmtVisitor& v) override {
		v.visit(*this);
	}
;};

class Block : public Stmt {
public:
	std::vector<Stmt*> statements;
public:
	Block(std::vector<Stmt*> statements) {
		this->statements = statements;
	}
	void accept(StmtVisitor& v) override {
		v.visit(*this);
	}
;};

class If : public Stmt {
public:
	Expr* condition;
	Stmt* thenBranch;
	Stmt* elseBranch;
public:
	If(Expr* condition, Stmt* thenBranch, Stmt* elseBranch) {
		this->condition = condition;
		this->thenBranch = thenBranch;
		this->elseBranch = elseBranch;
	}
	void accept(StmtVisitor& v) override {
		v.visit(*this);
	}
;};

class Var : public Stmt {
public:
	Token* name;
	Expr* right;
public:
	Var(Token* name, Expr* right) {
		this->name = name;
		this->right = right;
	}
	void accept(StmtVisitor& v) override {
		v.visit(*this);
	}
;};

