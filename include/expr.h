#ifndef EXPR_H
#define EXPR_H

#include <memory>
#include <string>
#include <vector>
#include <any>
#include <token.h>

class Binary;
class Grouping;
class Literal;
class Unary;

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    virtual std::any visitBinary(const class Binary& expr) = 0;
    virtual std::any visitGrouping(const class Grouping& expr) = 0;
    virtual std::any visitLiteral(const class Literal& expr) = 0;
    virtual std::any visitUnary(const class Unary& expr) = 0;
};

class Expr {
public:
    virtual ~Expr() = default;

    virtual std::any accept(ExprVisitor& visitor) const = 0;
};

class Binary : public Expr {
public:
    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);
    std::any accept(ExprVisitor& visitor) const override;

    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;
};

class Grouping : public Expr {
public:
    Grouping(std::shared_ptr<Expr> expression);
    std::any accept(ExprVisitor& visitor) const override;

    const std::shared_ptr<Expr> expression;
};

class Literal : public Expr {
public:
    Literal(std::any value);
    std::any accept(ExprVisitor& visitor) const override;

    const std::any value;
};

class Unary : public Expr {
public:
    Unary(Token op, std::shared_ptr<Expr> right);
    std::any accept(ExprVisitor& visitor) const override;

    const Token op;
    const std::shared_ptr<Expr> right;
};

#endif // EXPR_H
