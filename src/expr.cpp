#include "expr.h"

// -------- Binary --------
Binary::Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

std::any Binary::accept(Visitor& visitor) {
    return visitor.visitBinaryExpr(this);
}

// -------- Grouping --------
Grouping::Grouping(std::unique_ptr<Expr> expr)
    : expression(std::move(expr)) {}

std::any Grouping::accept(Visitor& visitor) {
    return visitor.visitGroupingExpr(this);
}

// -------- Literal --------
Literal::Literal(std::any value)
    : value(std::move(value)) {}

std::any Literal::accept(Visitor& visitor) {
    return visitor.visitLiteralExpr(this);
}

// -------- Unary --------
Unary::Unary(Token op, std::unique_ptr<Expr> right)
    : op(op), right(std::move(right)) {}

std::any Unary::accept(Visitor& visitor) {
    return visitor.visitUnaryExpr(this);
}
