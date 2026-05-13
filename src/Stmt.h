#pragma once

#include <vector>
#include <string>

#include <any>

#include "Token.h"

class Expression;
class Print;
class Return;
class Function;
class While;
class Block;
class If;
class Var;

class StmtVisitor {
public:
	virtual std::any visit(Expression& v) = 0;
	virtual std::any visit(Print& v) = 0;
	virtual std::any visit(Return& v) = 0;
	virtual std::any visit(Function& v) = 0;
	virtual std::any visit(While& v) = 0;
	virtual std::any visit(Block& v) = 0;
	virtual std::any visit(If& v) = 0;
	virtual std::any visit(Var& v) = 0;
};

class Stmt {
public:
	virtual std::any accept(StmtVisitor& v) = 0;
	~Stmt() = default;
};

class Expression : public Stmt {
public:
	Expr* expr;
public:
	Expression(Expr* expr) {
		this->expr = expr;
	}
	std::any accept(StmtVisitor& v) override {
		return v.visit(*this);
	}
;};

class Print : public Stmt {
public:
	Expr* expr;
public:
	Print(Expr* expr) {
		this->expr = expr;
	}
	std::any accept(StmtVisitor& v) override {
		return v.visit(*this);
	}
;};

class Return : public Stmt {
public:
	Token* keyword;
	Expr* value;
public:
	Return(Token* keyword, Expr* value) {
		this->keyword = keyword;
		this->value = value;
	}
	std::any accept(StmtVisitor& v) override {
		return v.visit(*this);
	}
;};

class Function : public Stmt {
public:
	Token* name;
	std::vector<Token*> params;
	std::vector<Stmt*> body;
public:
	Function(Token* name, std::vector<Token*> params, std::vector<Stmt*> body) {
		this->name = name;
		this->params = params;
		this->body = body;
	}
	std::any accept(StmtVisitor& v) override {
		return v.visit(*this);
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
	std::any accept(StmtVisitor& v) override {
		return v.visit(*this);
	}
;};

class Block : public Stmt {
public:
	std::vector<Stmt*> statements;
public:
	Block(std::vector<Stmt*> statements) {
		this->statements = statements;
	}
	std::any accept(StmtVisitor& v) override {
		return v.visit(*this);
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
	std::any accept(StmtVisitor& v) override {
		return v.visit(*this);
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
	std::any accept(StmtVisitor& v) override {
		return v.visit(*this);
	}
;};

