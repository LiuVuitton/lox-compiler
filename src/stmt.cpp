#include "stmt.h"

Expression::Expression(std::unique_ptr<Expr> expr)
    : expr(std::move(expr)) {}

std::any Expression::accept(Visitor& visitor) {
    return visitor.visitExpressionStmt(this);
}

Print::Print(std::unique_ptr<Expr> expr)
    : expr(std::move(expr)) {}

std::any Print::accept(Visitor& visitor) {
    return visitor.visitPrintStmt(this);
}

Var::Var(Token name, std::unique_ptr<Expr> initializer)
    : name(name), initializer(std::move(initializer)) {}

std::any Var::accept(Visitor& visitor) {
    return visitor.visitVarStmt(this);
}

