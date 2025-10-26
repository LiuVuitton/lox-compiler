#include "ast_printer.h"
#include <sstream>
#include <vector>
#include <any>

namespace lox {

std::string AstPrinter::print(const Expr& expr) {
    std::any result = const_cast<Expr&>(expr).accept(*this);
    return std::any_cast<std::string>(result);
}

std::any AstPrinter::visitBinaryExpr(Binary* expr) {
    std::vector<Expr*> exprs = { expr->left.get(), expr->right.get() };
    return parenthesize(expr->op.lexeme, exprs);
}

std::any AstPrinter::visitGroupingExpr(Grouping* expr) {
    std::vector<Expr*> exprs = { expr->expr.get() };
    return parenthesize("group", exprs);
}

std::any AstPrinter::visitLiteralExpr(Literal* expr) {
    if (expr->value.type() == typeid(std::nullptr_t)) {
        return std::string("nil");
    }
    
    // Try to get the string value
    try {
        return std::any_cast<std::string>(expr->value);
    } catch (...) {
        try {
            return std::to_string(std::any_cast<double>(expr->value));
        } catch (...) {
            try {
                return std::to_string(std::any_cast<int>(expr->value));
            } catch (...) {
                try {
                    return expr->value.type().name(); // Type name as fallback
                } catch (...) {
                    return std::string("<unknown>");
                }
            }
        }
    }
}

std::any AstPrinter::visitUnaryExpr(Unary* expr) {
    std::vector<Expr*> exprs = { expr->right.get() };
    return parenthesize(expr->op.lexeme, exprs);
}

std::string AstPrinter::parenthesize(const std::string& name, const std::vector<Expr*>& exprs) {
    std::ostringstream builder;
    builder << "(" << name;
    
    for (const auto& expr : exprs) {
        if (expr) {
            builder << " ";
            std::any result = expr->accept(*this);
            builder << std::any_cast<std::string>(result);
        }
    }
    
    builder << ")";
    return builder.str();
}

}
