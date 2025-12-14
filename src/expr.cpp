#include "expr.h"

Assign::Assign(Token name, std::unique_ptr<Expr> value)
    : name(name), value(std::move(value)) {}

std::any Assign::accept(Visitor& visitor) {
    return visitor.visitAssignExpr(this);
}

Binary::Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

std::any Binary::accept(Visitor& visitor) {
    return visitor.visitBinaryExpr(this);
}

Grouping::Grouping(std::unique_ptr<Expr> expr)
    : expr(std::move(expr)) {}

std::any Grouping::accept(Visitor& visitor) {
    return visitor.visitGroupingExpr(this);
}

Literal::Literal(std::any value)
    : value(value) {}

std::any Literal::accept(Visitor& visitor) {
    return visitor.visitLiteralExpr(this);
}

Unary::Unary(Token op, std::unique_ptr<Expr> right)
    : op(op), right(std::move(right)) {}

std::any Unary::accept(Visitor& visitor) {
    return visitor.visitUnaryExpr(this);
}

Variable::Variable(Token name)
    : name(name) {}

std::any Variable::accept(Visitor& visitor) {
    return visitor.visitVariableExpr(this);
}

