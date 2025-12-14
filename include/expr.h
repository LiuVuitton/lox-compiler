#ifndef EXPR_H
#define EXPR_H

#include <any>
#include <memory>
#include "token.h"

class Assign;
class Binary;
class Grouping;
class Literal;
class Unary;
class Variable;

class Expr {
public:
    class Visitor {
    public:
        virtual ~Visitor() = default;
        virtual std::any visitAssignExpr(Assign* expr) = 0;
        virtual std::any visitBinaryExpr(Binary* expr) = 0;
        virtual std::any visitGroupingExpr(Grouping* expr) = 0;
        virtual std::any visitLiteralExpr(Literal* expr) = 0;
        virtual std::any visitUnaryExpr(Unary* expr) = 0;
        virtual std::any visitVariableExpr(Variable* expr) = 0;
    };

    virtual ~Expr() = default;
    virtual std::any accept(Visitor& visitor) = 0;
};

class Assign : public Expr {
public:
    Token name;
    std::unique_ptr<Expr> value;

    explicit Assign(Token name, std::unique_ptr<Expr> value);
    std::any accept(Visitor& visitor) override;
};

class Binary : public Expr {
public:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    explicit Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);
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

    explicit Unary(Token op, std::unique_ptr<Expr> right);
    std::any accept(Visitor& visitor) override;
};

class Variable : public Expr {
public:
    Token name;

    explicit Variable(Token name);
    std::any accept(Visitor& visitor) override;
};

#endif // EXPR_H
