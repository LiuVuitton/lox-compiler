#ifndef EXPR_H
#define EXPR_H

#include <any>
#include <memory>
#include "token.h"

namespace lox {

class Expr {
public:
    class Visitor {
    public:
        virtual ~Visitor() = default;
        virtual std::any visitBinaryExpr(class Binary* expr) = 0;
        virtual std::any visitGroupingExpr(class Grouping* expr) = 0;
        virtual std::any visitLiteralExpr(class Literal* expr) = 0;
        virtual std::any visitUnaryExpr(class Unary* expr) = 0;
    };

    virtual ~Expr() = default;
    virtual std::any accept(Visitor& visitor) = 0; 
};

// ---------- Expression types ----------
class Binary : public Expr {
public:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    std::any accept(Visitor& visitor) override {
        return visitor.visitBinaryExpr(this);
    }
};

class Grouping : public Expr {
public:
    std::unique_ptr<Expr> expr;

    Grouping(std::unique_ptr<Expr> expr)
        : expr(std::move(expr)) {}
    
    std::any accept(Visitor& visitor) override {
        return visitor.visitGroupingExpr(this);
    }
};

class Literal : public Expr {
public:
    std::any value;

    Literal(std::any value)
        : value(value) {}
    
    std::any accept(Visitor& visitor) override {
        return visitor.visitLiteralExpr(this);
    }
};

class Unary : public Expr {
public:
    Token op;
    std::unique_ptr<Expr> right;

    Unary(Token op, std::unique_ptr<Expr> right)
        : op(op), right(std::move(right)) {}
    std::any accept(Visitor& visitor) override {
        return visitor.visitUnaryExpr(this);
    }
};

} // namespace lox

#endif // EXPR_H