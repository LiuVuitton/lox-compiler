#include <iostream>
#include <cmath>
#include <stdexcept>
#include "interpreter.h"
#include "runtime_error.h"

namespace lox {

void Interpreter::interpret(std::unique_ptr<Expr>& expression) {
    try {
        Value value = std::any_cast<Value>(expression->accept(*this));
        std::cout << stringify(value) << std::endl;
    } catch (const RuntimeError& error) {
        std::cerr << error.what() << " [line " << error.token.line << "]" << std::endl;
    }
}

// Literal
std::any Interpreter::visitLiteralExpr(Literal* expr) {
    return expr->value;
}

// Grouping
std::any Interpreter::visitGroupingExpr(Grouping* expr) {
    return expr->expr->accept(*this);
}

// Unary
std::any Interpreter::visitUnaryExpr(Unary* expr) {
    std::any rightAny = expr->right->accept(*this);
    Value right = std::any_cast<Value>(rightAny);

    switch (expr->op.type) {
        case TokenType::MINUS:
            checkNumberOperand(expr->op, right);
            return -std::get<double>(right);
        case TokenType::BANG:
            return !isTruthy(right);
        default:
            return std::monostate{}; // unreachable
    }
}

// Binary
std::any Interpreter::visitBinaryExpr(Binary* expr) {
    std::any leftAny = expr->left->accept(*this);
    std::any rightAny = expr->right->accept(*this);
    Value left = std::any_cast<Value>(leftAny);
    Value right = std::any_cast<Value>(rightAny);

    switch (expr->op.type) {
        case TokenType::PLUS:
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) + std::get<double>(right);
            }
            if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                return std::get<std::string>(left) + std::get<std::string>(right);
            }
            throw RuntimeError(expr->op, "Operands must be two numbers or two strings.");
        case TokenType::MINUS:
            checkNumberOperands(expr->op, left, right);
            return std::get<double>(left) - std::get<double>(right);
        case TokenType::STAR:
            checkNumberOperands(expr->op, left, right);
            return std::get<double>(left) * std::get<double>(right);
        case TokenType::SLASH:
            checkNumberOperands(expr->op, left, right);
            return std::get<double>(left) / std::get<double>(right);
        case TokenType::GREATER:
            checkNumberOperands(expr->op, left, right);
            return std::get<double>(left) > std::get<double>(right);
        case TokenType::GREATER_EQUAL:
            checkNumberOperands(expr->op, left, right);
            return std::get<double>(left) >= std::get<double>(right);
        case TokenType::LESS:
            checkNumberOperands(expr->op, left, right);
            return std::get<double>(left) < std::get<double>(right);
        case TokenType::LESS_EQUAL:
            checkNumberOperands(expr->op, left, right);
            return std::get<double>(left) <= std::get<double>(right);
        case TokenType::EQUAL_EQUAL:
            return isEqual(left, right);
        case TokenType::BANG_EQUAL:
            return !isEqual(left, right);
        default:
            return std::monostate{}; // unreachable
    }
}

// Helpers
bool Interpreter::isTruthy(const Value& value) {
    if (std::holds_alternative<std::monostate>(value)) return false;
    if (std::holds_alternative<bool>(value)) return std::get<bool>(value);
    return true;
}

bool Interpreter::isEqual(const Value& a, const Value& b) {
    if (std::holds_alternative<std::monostate>(a) && std::holds_alternative<std::monostate>(b)) return true;
    if (std::holds_alternative<std::monostate>(a) || std::holds_alternative<std::monostate>(b)) return false;
    return a == b;
}

void Interpreter::checkNumberOperand(const Token& op, const Value& operand) {
    if (std::holds_alternative<double>(operand)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(const Token& op, const Value& left, const Value& right) {
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
    throw RuntimeError(op, "Operands must be numbers.");
}

std::string Interpreter::stringify(const Value& value) {
    if (std::holds_alternative<std::monostate>(value)) return "nil";
    if (std::holds_alternative<double>(value)) {
        double num = std::get<double>(value);
        std::string text = std::to_string(num);
        if (text.find(".0") != std::string::npos) {
            text = text.substr(0, text.find(".0"));
        }
        return text;
    }
    if (std::holds_alternative<std::string>(value)) return std::get<std::string>(value);
    if (std::holds_alternative<bool>(value)) return std::get<bool>(value) ? "true" : "false";
    return "";
}

} // namespace lox