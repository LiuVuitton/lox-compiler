#include "ast_printer.h"

std::string AstPrinter::print(Expr* expr) {
    std::any result = expr->accept(*this);
    return std::any_cast<std::string>(result);
}

std::any AstPrinter::visitBinaryExpr(Binary* expr) {
    std::vector<Expr*> children = { expr->left.get(), expr->right.get() };
    return parenthesize(expr->op.lexeme, children);
}

std::any AstPrinter::visitGroupingExpr(Grouping* expr) {
    std::vector<Expr*> children = { expr->expr.get() };
    return parenthesize("group", children);
}

std::any AstPrinter::visitLiteralExpr(Literal* expr) {
    if (expr->value.type() == typeid(std::nullptr_t)) return std::string("nil");

    try {
        return std::any_cast<std::string>(expr->value);
    } catch (...) {
        try { return std::to_string(std::any_cast<double>(expr->value)); }
        catch (...) {
            try { return std::to_string(std::any_cast<int>(expr->value)); }
            catch (...) { return "<unknown>"; }
        }
    }
}

std::any AstPrinter::visitUnaryExpr(Unary* expr) {
    std::vector<Expr*> children = { expr->right.get() };
    return parenthesize(expr->op.lexeme, children);
}

std::string AstPrinter::parenthesize(const std::string& name, const std::vector<Expr*>& exprs) {
    std::ostringstream builder;
    builder << "(" << name;

    for (Expr* e : exprs) {
        builder << " ";
        std::any result = e->accept(*this);
        builder << std::any_cast<std::string>(result);
    }

    builder << ")";
    return builder.str();
}
