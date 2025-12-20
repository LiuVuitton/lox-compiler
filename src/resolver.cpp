#include "resolver.h"
#include "lox.h"

Resolver::Resolver(Interpreter& interpreter) : interpreter(interpreter) {}

void Resolver::resolve(const std::vector<std::unique_ptr<Stmt>>& statements) {
    for (const auto& stmt : statements) {
        resolve(stmt.get());
    }
}

void Resolver::resolve(Stmt* stmt) {
    stmt->accept(*this);
}

void Resolver::resolve(Expr* expr) {
    expr->accept(*this);
}

void Resolver::resolveFunction(Function* function, FunctionType type) {
    FunctionType enclosing_function = current_function;
    current_function = type;

    beginScope();
    for (const Token& param : function->params) {
        declare(param);
        define(param);
    }
    resolve(function->body);
    endScope();
    current_function = enclosing_function;
}

void Resolver::beginScope() {
    scopes.push_back({});
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::declare(const Token& name) {
    if (scopes.empty()) return;
    auto scope = scopes.back();
    if (scope.count(name.lexeme)) {
        Lox::error(name, "Already variable with this name in this scope.");
    }
    scopes.back()[name.lexeme] = false;
}

void Resolver::define(const Token& name) {
    if (scopes.empty()) return;
    scopes.back()[name.lexeme] = true;
}

void Resolver::resolveLocal(Expr* expr, const Token& name) {
    for (int i = static_cast<int>(scopes.size()) - 1; i >= 0; --i) {
        auto& scope = scopes[i];
        if (scope.find(name.lexeme) != scope.end()) {
            int distance = static_cast<int>(scopes.size() - 1 - i);
            interpreter.resolve(expr, distance);
            return;
        }
    }
}

// ---------------------- Stmt Visitors ----------------------

std::any Resolver::visitBlockStmt(Block* stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
    return {};
}

std::any Resolver::visitExpressionStmt(Expression* stmt) {
    resolve(stmt->expr.get());
    return {};
}

std::any Resolver::visitIfStmt(If* stmt) {
    resolve(stmt->condition.get());
    resolve(stmt->then_branch.get());
    if (stmt->else_branch) resolve(stmt->else_branch.get());
    return {};
}

std::any Resolver::visitPrintStmt(Print* stmt) {
    resolve(stmt->expr.get());
    return {};
}

std::any Resolver::visitReturnStmt(Return* stmt) {
    if (current_function == FunctionType::NONE) {
        Lox::error(stmt->keyword, "Can't return from top-level code.");
    }
    
    if (stmt->value) resolve(stmt->value.get());
    return {};
}

std::any Resolver::visitVarStmt(Var* stmt) {
    declare(stmt->name);
    if (stmt->initializer) resolve(stmt->initializer.get());
    define(stmt->name);
    return {};
}

std::any Resolver::visitWhileStmt(While* stmt) {
    resolve(stmt->condition.get());
    resolve(stmt->body.get());
    return {};
}

std::any Resolver::visitFunctionStmt(Function* stmt) {
    declare(stmt->name);
    define(stmt->name);
    resolveFunction(stmt, FunctionType::FUNCTION);
    return {};
}

// ---------------------- Expr Visitors ----------------------

std::any Resolver::visitAssignExpr(Assign* expr) {
    resolve(expr->value.get());
    resolveLocal(expr, expr->name);
    return {};
}

std::any Resolver::visitBinaryExpr(Binary* expr) {
    resolve(expr->left.get());
    resolve(expr->right.get());
    return {};
}

std::any Resolver::visitCallExpr(Call* expr) {
    resolve(expr->callee.get());
    for (const auto& arg : expr->arguments) {
        resolve(arg.get());
    }
    return {};
}

std::any Resolver::visitGroupingExpr(Grouping* expr) {
    resolve(expr->expr.get());
    return {};
}

std::any Resolver::visitLiteralExpr(Literal* /*expr*/) {
    return {};
}

std::any Resolver::visitLogicalExpr(Logical* expr) {
    resolve(expr->left.get());
    resolve(expr->right.get());
    return {};
}

std::any Resolver::visitUnaryExpr(Unary* expr) {
    resolve(expr->right.get());
    return {};
}

std::any Resolver::visitVariableExpr(Variable* expr) {
    if (!scopes.empty() && scopes.back()[expr->name.lexeme] == false) {
        Lox::error(expr->name, "Can't read local variable in its own initializer.");
    }
    resolveLocal(expr, expr->name);
    return {};
}
