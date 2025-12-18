#include "interpreter.h"
#include "runtime_error.h"
#include "lox.h"
#include <sstream>
#include <iostream>
#include <vector>

Interpreter::Interpreter()
    : environment(std::make_shared<Environment>()) {}

/*
void Interpreter::interpret(Expr* expr) {
    try {
        std::any value = evaluate(expr);
        std::cout << stringify(value) << "\n";
    }
    catch (RuntimeError error) {
        Lox::runtimeError(error);
    }
}
    */

void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>>& statements) {
    try {
        for (const auto& statement : statements) {
            execute(statement.get());
        }
    } catch (const RuntimeError& error) {
        Lox::runtimeError(error);
    }
}

std::any Interpreter::visitLiteralExpr(Literal* expr) {
    return expr->value;
}

std::any Interpreter::visitLogicalExpr(Logical* expr) {
    std::any left = evaluate(expr->left.get());

    if (expr->op.type == TokenType::OR) {
        if (isTruthy(left)) return left;
    }
    else {
        if (!isTruthy(left)) return left;
    }
    
    return evaluate(expr->right.get());
}

std::any Interpreter::visitGroupingExpr(Grouping* expr) {
    return evaluate(expr->expr.get());
}

std::any Interpreter::visitUnaryExpr(Unary* expr) {
    std::any right = evaluate(expr->right.get());

    switch (expr->op.type) {
        case TokenType::BANG:
            return !isTruthy(right);
        case TokenType::MINUS:
            checkNumberOperand(expr->op, right);
            return -std::any_cast<double>(right);
    }
    return std::any{};
}

std::any Interpreter::visitVariableExpr(Variable* expr) {
    return environment->get(expr->name);
}

std::any Interpreter::visitExpressionStmt(Expression* stmt) {
    evaluate(stmt->expr.get());
    return std::any{};
}

std::any Interpreter::visitIfStmt(If* stmt) {
    if (isTruthy(evaluate(stmt->condition.get()))) {
        execute(stmt->then_branch.get());
    }
    else if (stmt->else_branch) {
        execute(stmt->else_branch.get());
    }
    return std::any{};
}

std::any Interpreter::visitPrintStmt(Print* stmt) {
    std::any value = evaluate(stmt->expr.get());
    std::cout << stringify(value) << "\n";
    return std::any{};
}

std::any Interpreter::visitVarStmt(Var* stmt) {
    std::any value;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer.get());
    }
    environment->define(stmt->name.lexeme, value);
    return std::any{};
}

std::any Interpreter::visitAssignExpr(Assign* expr) {
    std::any value = evaluate(expr->value.get());
    environment->assign(expr->name, value);
    return value;
}

std::any Interpreter::visitBlockStmt(Block* stmt) {
    executeBlock(stmt->statements, std::make_shared<Environment>(environment));
    return std::any{};
}

void Interpreter::checkNumberOperand(const Token& op, const std::any& operand) {
    if (operand.type() == typeid(double)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

std::any Interpreter::visitBinaryExpr(Binary* expr) {
    std::any left = evaluate(expr->left.get());
    std::any right = evaluate(expr->right.get());

    switch (expr->op.type) {
        case TokenType::BANG_EQUAL: return !isEqual(left, right);
        case TokenType::EQUAL_EQUAL: return isEqual(left, right);
        case TokenType::GREATER:
            checkNumberOperands(expr->op, left, right);
            return std::any_cast<double>(left) > std::any_cast<double>(right);

        case TokenType::GREATER_EQUAL:
            checkNumberOperands(expr->op, left, right);
            return std::any_cast<double>(left) >= std::any_cast<double>(right);

        case TokenType::LESS:
            checkNumberOperands(expr->op, left, right);
            return std::any_cast<double>(left) < std::any_cast<double>(right);

        case TokenType::LESS_EQUAL:
            checkNumberOperands(expr->op, left, right);
            return std::any_cast<double>(left) <= std::any_cast<double>(right);

        case TokenType::MINUS:
            checkNumberOperands(expr->op, left, right);
            return std::any_cast<double>(left) - std::any_cast<double>(right);

        case TokenType::PLUS:
            if (left.type() == typeid(double) && right.type() == typeid(double)) {
                return std::any_cast<double>(left) + std::any_cast<double>(right);
            }
            if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            }
            throw RuntimeError(expr->op, "Operands must be two numbers or two strings.");

        case TokenType::SLASH:
            checkNumberOperands(expr->op, left, right);
            return std::any_cast<double>(left) / std::any_cast<double>(right);

        case TokenType::STAR:
            checkNumberOperands(expr->op, left, right);
            return std::any_cast<double>(left) * std::any_cast<double>(right);
        
    }

    return std::any{};
}

void Interpreter::checkNumberOperands(const Token& op, const std::any& left, const std::any& right) {
    if (left.type() == typeid(double) && right.type() == typeid(double)) return;
    throw RuntimeError(op, "Operands must be numbers.");
}

std::any Interpreter::evaluate(Expr* expr) {
    return expr->accept(*this);
}

bool Interpreter::isTruthy(const std::any& value) {
    if (!value.has_value()) return false;

    if (value.type() == typeid(bool)) {
        return std::any_cast<bool>(value);
    }

    return true;
}

bool Interpreter::isEqual(const std::any& a, const std::any& b) {
    if (!a.has_value() && !b.has_value()) return true;

    if (!a.has_value() || !b.has_value()) return false;

    if (a.type() == b.type()) {
        if (a.type() == typeid(double)) {
            return std::any_cast<double>(a) == std::any_cast<double>(b);
        } else if (a.type() == typeid(std::string)) {
            return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
        } else if (a.type() == typeid(bool)) {
            return std::any_cast<bool>(a) == std::any_cast<bool>(b);
        }
    }

    return false;
}

std::string Interpreter::stringify(const std::any& object) {
    if (!object.has_value()) return "nil";

    if (object.type() == typeid(double)) {
        std::string text = std::to_string(std::any_cast<double>(object));

        if (text.size() >= 2 &&
            text.compare(text.size() - 2, 2, ".0") == 0) {
            text.erase(text.size() - 2);
        }

        return text;
    }

    if (object.type() == typeid(std::string)) {
        return std::any_cast<std::string>(object);
    }

    if (object.type() == typeid(bool)) {
        return std::any_cast<bool>(object) ? "true" : "false";
    }

    return "null";
}

void Interpreter::execute(Stmt* stmt) {
    stmt->accept(*this);
}

void Interpreter::executeBlock(
    const std::vector<std::unique_ptr<Stmt>>& statements,
    std::shared_ptr<Environment> newEnv
) {
    std::shared_ptr<Environment> previous = environment;
    environment = std::move(newEnv);

    try {
        for (const auto& stmt : statements) {
            execute(stmt.get());
        }
    } catch (...) {
        environment = previous;
        throw;
    }

    environment = previous;
}

