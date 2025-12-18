#ifndef STMT_H
#define STMT_H

#include <any>
#include <memory>
#include "token.h"
#include "expr.h"

class Block;
class Expression;
class If;
class Print;
class Var;

class Stmt {
public:
    class Visitor {
    public:
        virtual ~Visitor() = default;
        virtual std::any visitBlockStmt(Block* stmt) = 0;
        virtual std::any visitExpressionStmt(Expression* stmt) = 0;
        virtual std::any visitIfStmt(If* stmt) = 0;
        virtual std::any visitPrintStmt(Print* stmt) = 0;
        virtual std::any visitVarStmt(Var* stmt) = 0;
    };

    virtual ~Stmt() = default;
    virtual std::any accept(Visitor& visitor) = 0;
};

class Block : public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> statements;

    explicit Block(std::vector<std::unique_ptr<Stmt>> statements);
    std::any accept(Visitor& visitor) override;
};

class Expression : public Stmt {
public:
    std::unique_ptr<Expr> expr;

    explicit Expression(std::unique_ptr<Expr> expr);
    std::any accept(Visitor& visitor) override;
};

class If : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> then_branch;
    std::unique_ptr<Stmt> else_branch;

    explicit If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_branch, std::unique_ptr<Stmt> else_branch);
    std::any accept(Visitor& visitor) override;
};

class Print : public Stmt {
public:
    std::unique_ptr<Expr> expr;

    explicit Print(std::unique_ptr<Expr> expr);
    std::any accept(Visitor& visitor) override;
};

class Var : public Stmt {
public:
    Token name;
    std::unique_ptr<Expr> initializer;

    explicit Var(Token name, std::unique_ptr<Expr> initializer);
    std::any accept(Visitor& visitor) override;
};

#endif // STMT_H
