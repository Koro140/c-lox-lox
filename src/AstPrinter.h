#include "Expr.h"

class AstPrinter : public Visitor
{
private:
    std::string result;
    
    template<typename... TExpr>
    void parenthesize(std::string name, std::vector<Expr*> exprs) {

        result.append("(").append(name);

        for (Expr* expr : exprs) {
            expr->accept(*this);
            std::cout << "he";
        }

        result.append(")");
    }
public:
	void visit(Grouping& v) {
        parenthesize("group", {v.expr});
    }
	
    void visit(Binary& v) {
        parenthesize(v.op->getLexeme(), {v.left, v.right});
    }
	
    void visit(Unary& v) {
        parenthesize(v.op->getLexeme(), {v.right});
    }

	void visit(Literal& v) {
        if (v.value.type() == typeid(nullptr)) {
            result.append("nil");
        }
        else if (v.value.type() == typeid(double)){
            result.append(std::to_string(std::any_cast<double>(v.value)));
            std::cout << std::any_cast<double>(v.value);
        }
        else if (v.value.type() == typeid(std::string)) {
            result.append(std::any_cast<std::string>(v.value));
        }
    }

    std::string getResult() {
        return result;
    }
};