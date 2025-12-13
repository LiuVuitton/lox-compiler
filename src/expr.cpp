#include <expr.h>

Binary::Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) : left(left), op(op), right(right) {}

std::any Binary::accept(ExprVisitor& visitor) const {
    return visitor.visitBinary(*this);
}

Grouping::Grouping(std::unique_ptr<Expr> expression) : expression(expression) {}

std::any Grouping::accept(ExprVisitor& visitor) const {
    return visitor.visitGrouping(*this);
}

Literal::Literal(std::any value) : value(value) {}

std::any Literal::accept(ExprVisitor& visitor) const {
    return visitor.visitLiteral(*this);
}

Unary::Unary(Token op, std::unique_ptr<Expr> right) : op(op), right(right) {}

std::any Unary::accept(ExprVisitor& visitor) const {
    return visitor.visitUnary(*this);
}

