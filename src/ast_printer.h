#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "expr.h"
#include <string>
#include <vector>

namespace lox {

class AstPrinter : public Expr::Visitor {
public:
    std::string print(const Expr& expr);
private:
    std::any visitBinaryExpr(Binary* expr) override;
    std::any visitGroupingExpr(Grouping* expr) override;
    std::any visitLiteralExpr(Literal* expr) override;
    std::any visitUnaryExpr(Unary* expr) override;
    
    std::string parenthesize(const std::string& name, const std::vector<Expr*>& exprs);
};

}

#endif // AST_PRINTER_H
