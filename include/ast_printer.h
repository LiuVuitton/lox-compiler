#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "expr.h"
#include <string>
#include <vector>
#include <any>
#include <sstream>

class AstPrinter : public Expr::Visitor {
public:
    AstPrinter() = default;
    ~AstPrinter() override = default;

    // Entry point: print the expression as a string
    std::string print(Expr* expr);

    // Visitor overrides
    std::any visitBinaryExpr(Binary* expr) override;
    std::any visitGroupingExpr(Grouping* expr) override;
    std::any visitLiteralExpr(Literal* expr) override;
    std::any visitUnaryExpr(Unary* expr) override;

private:
    // Helper to parenthesize expressions like: (operator left right)
    std::string parenthesize(const std::string& name, const std::vector<Expr*>& exprs);
};

#endif // AST_PRINTER_H
