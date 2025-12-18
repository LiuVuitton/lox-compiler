#include "stmt.h"
#include <vector>

Block::Block(std::vector<std::unique_ptr<Stmt>> statements)
    : statements(std::move(statements)) {}

std::any Block::accept(Visitor& visitor) {
    return visitor.visitBlockStmt(this);
}

Expression::Expression(std::unique_ptr<Expr> expr)
    : expr(std::move(expr)) {}

std::any Expression::accept(Visitor& visitor) {
    return visitor.visitExpressionStmt(this);
}

If::If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_branch, std::unique_ptr<Stmt> else_branch)
    : condition(std::move(condition)), then_branch(std::move(then_branch)), else_branch(std::move(else_branch)) {}

std::any If::accept(Visitor& visitor) {
    return visitor.visitIfStmt(this);
}

Print::Print(std::unique_ptr<Expr> expr)
    : expr(std::move(expr)) {}

std::any Print::accept(Visitor& visitor) {
    return visitor.visitPrintStmt(this);
}

Var::Var(Token name, std::unique_ptr<Expr> initializer)
    : name(name), initializer(std::move(initializer)) {}

std::any Var::accept(Visitor& visitor) {
    return visitor.visitVarStmt(this);
}

