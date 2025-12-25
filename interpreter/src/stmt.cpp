#include "stmt.h"

Block::Block(std::vector<std::unique_ptr<Stmt>> statements)
    : statements(std::move(statements)) {}

std::any Block::accept(Visitor& visitor) {
    return visitor.visitBlockStmt(this);
}

Class::Class(Token name, std::shared_ptr<Variable> superclass, std::vector<std::unique_ptr<Function>> methods)
    : name(name), superclass(superclass), methods(std::move(methods)) {}

std::any Class::accept(Visitor& visitor) {
    return visitor.visitClassStmt(this);
}

Expression::Expression(std::unique_ptr<Expr> expr)
    : expr(std::move(expr)) {}

std::any Expression::accept(Visitor& visitor) {
    return visitor.visitExpressionStmt(this);
}

Function::Function(Token name, std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body)
    : name(name), params(params), body(std::move(body)) {}

std::any Function::accept(Visitor& visitor) {
    return visitor.visitFunctionStmt(this);
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

Return::Return(Token keyword, std::unique_ptr<Expr> value)
    : keyword(keyword), value(std::move(value)) {}

std::any Return::accept(Visitor& visitor) {
    return visitor.visitReturnStmt(this);
}

While::While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
    : condition(std::move(condition)), body(std::move(body)) {}

std::any While::accept(Visitor& visitor) {
    return visitor.visitWhileStmt(this);
}

Var::Var(Token name, std::unique_ptr<Expr> initializer)
    : name(name), initializer(std::move(initializer)) {}

std::any Var::accept(Visitor& visitor) {
    return visitor.visitVarStmt(this);
}

