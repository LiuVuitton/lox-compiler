#ifndef EXPR_H
#define EXPR_H

#include <any>
#include <memory>
#include "token.h"

namespace lox {

class Expr {
public:
    virtual ~Expr() = default;

    template<typename R>
    class Visitor {
    public:
        virtual ~Visitor() = default;
        virtual R visitBinaryExpr(Binary* expr) = 0;
        virtual R visitGroupingExpr(Grouping* expr) = 0;
        virtual R visitLiteralExpr(Literal* expr) = 0;
        virtual R visitUnaryExpr(Unary* expr) = 0;
    };

    template<typename R>
    virtual R accept(Visitor<R>& visitor) = 0;
};

class Binary : public Expr {
public:
    Binary(Expr* left, Token operator, Expr* right) : left(left), operator(operator), right(right) {}

    Expr* left;
    Token operator;
    Expr* right;

    template<typename R>
    R accept(Visitor<R>& visitor) override {
        return visitor.visitBinaryExpr(this);
    }
};

class Grouping : public Expr {
public:
    Grouping(Expr* expression) : expression(expression) {}

    Expr* expression;

    template<typename R>
    R accept(Visitor<R>& visitor) override {
        return visitor.visitGroupingExpr(this);
    }
};

class Literal : public Expr {
public:
    Literal(std::any value) : value(value) {}

    std::any value;

    template<typename R>
    R accept(Visitor<R>& visitor) override {
        return visitor.visitLiteralExpr(this);
    }
};

class Unary : public Expr {
public:
    Unary(Token operator, Expr* right) : operator(operator), right(right) {}

    Token operator;
    Expr* right;

    template<typename R>
    R accept(Visitor<R>& visitor) override {
        return visitor.visitUnaryExpr(this);
    }
};

} // namespace lox

#endif // EXPR_H
