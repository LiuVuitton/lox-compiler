#include <expr.h>

Binary::Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left(left), op(op), right(right) {}

std::any Binary::accept(ExprVisitor& visitor) const {
    return visitor.visitBinary(*this);
}

#include <expr.h>

Grouping::Grouping(std::shared_ptr<Expr> expression) : expression(expression) {}

std::any Grouping::accept(ExprVisitor& visitor) const {
    return visitor.visitGrouping(*this);
}

#include <expr.h>

Literal::Literal(std::any value) : value(value) {}

std::any Literal::accept(ExprVisitor& visitor) const {
    return visitor.visitLiteral(*this);
}

#include <expr.h>

Unary::Unary(Token op, std::shared_ptr<Expr> right) : op(op), right(right) {}

std::any Unary::accept(ExprVisitor& visitor) const {
    return visitor.visitUnary(*this);
}

