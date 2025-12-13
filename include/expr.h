#ifndef EXPR_H
#define EXPR_H

#include <any>
#include <memory>
#include "token.h"

class Binary;
class Grouping;
class Literal;
class Unary;

class Expr {
public:
    class Visitor {
    public:
        virtual ~Visitor() = default;
        virtual std::any visitBinaryExpr(Binary* expr) = 0;
        virtual std::any visitGroupingExpr(Grouping* expr) = 0;
        virtual std::any visitLiteralExpr(Literal* expr) = 0;
        virtual std::any visitUnaryExpr(Unary* expr) = 0;
    };

    virtual ~Expr() = default;
    virtual std::any accept(Visitor& visitor) = 0;
};

class Binary : public Expr {
public:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);

    std::any accept(Visitor& visitor) override;
};

class Grouping : public Expr {
public:
    std::unique_ptr<Expr> expr;

    explicit Grouping(std::unique_ptr<Expr> expr);

    std::any accept(Visitor& visitor) override;
};

class Literal : public Expr {
public:
    std::any value;

    explicit Literal(std::any value);

    std::any accept(Visitor& visitor) override;
};

class Unary : public Expr {
public:
    Token op;
    std::unique_ptr<Expr> right;

    Unary(Token op, std::unique_ptr<Expr> right);

    std::any accept(Visitor& visitor) override;
};

#endif // EXPR_H
