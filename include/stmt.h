#ifndef STMT_H
#define STMT_H

#include <any>
#include <memory>
#include "token.h"
#include "expr.h"

class Expression;
class Print;

class Stmt {
public:
    class Visitor {
    public:
        virtual ~Visitor() = default;
        virtual std::any visitExpressionStmt(Expression* stmt) = 0;
        virtual std::any visitPrintStmt(Print* stmt) = 0;
    };

    virtual ~Stmt() = default;
    virtual std::any accept(Visitor& visitor) = 0;
};

class Expression : public Stmt {
public:
    std::unique_ptr<Expr> expr;

    explicit Expression(std::unique_ptr<Expr> expr);
    std::any accept(Visitor& visitor) override;
};

class Print : public Stmt {
public:
    std::unique_ptr<Expr> expr;

    explicit Print(std::unique_ptr<Expr> expr);
    std::any accept(Visitor& visitor) override;
};

#endif // STMT_H
