#include "interpreter.h"

#include "runtime_error.h"
#include "lox.h"
#include "lox_callable.h"
#include "lox_function.h"
#include "function_return.h"
#include "native_function.h"

#include <iostream>
#include <chrono>

Interpreter::Interpreter()
    : globals(std::make_shared<Environment>()),
      environment(globals)
{
    globals->define(
        "clock",
        std::make_any<std::shared_ptr<LoxCallable>>(
            std::make_shared<NativeFunction>(
                [](Interpreter&, const std::vector<std::any>&) -> std::any {
                    auto now = std::chrono::system_clock::now();
                    auto epoch = now.time_since_epoch();
                    return std::chrono::duration<double>(epoch).count();
                },
                0
            )
        )
    );
}

void Interpreter::interpret(
    const std::vector<std::unique_ptr<Stmt>>& statements
) {
    try {
        for (const auto& stmt : statements) {
            execute(stmt.get());
        }
    } catch (const RuntimeError& error) {
        Lox::runtimeError(error);
    }
}

/* =======================
   Expression evaluation
   ======================= */

std::any Interpreter::evaluate(Expr* expr) {
    return expr->accept(*this);
}

std::any Interpreter::visitLiteralExpr(Literal* expr) {
    return expr->value;
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
        default:
            return {};
    }
}

std::any Interpreter::visitLogicalExpr(Logical* expr) {
    std::any left = evaluate(expr->left.get());

    if (expr->op.type == TokenType::OR) {
        if (isTruthy(left)) return left;
    } else {
        if (!isTruthy(left)) return left;
    }

    return evaluate(expr->right.get());
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
            if (left.type() == typeid(double) &&
                right.type() == typeid(double)) {
                return std::any_cast<double>(left) +
                       std::any_cast<double>(right);
            }
            if (left.type() == typeid(std::string) &&
                right.type() == typeid(std::string)) {
                return std::any_cast<std::string>(left) +
                       std::any_cast<std::string>(right);
            }
            throw RuntimeError(expr->op,
                "Operands must be two numbers or two strings.");

        case TokenType::SLASH:
            checkNumberOperands(expr->op, left, right);
            return std::any_cast<double>(left) /
                   std::any_cast<double>(right);

        case TokenType::STAR:
            checkNumberOperands(expr->op, left, right);
            return std::any_cast<double>(left) *
                   std::any_cast<double>(right);

        default:
            return {};
    }
}

std::any Interpreter::visitVariableExpr(Variable* expr) {
    return lookUpVariable(expr->name, expr);
}

std::any Interpreter::visitAssignExpr(Assign* expr) {
    std::any value = evaluate(expr->value.get());

    auto it = locals.find(expr);
    if (it != locals.end()) {
        int distance = it->second;
        environment->assignAt(distance, expr->name, value);
    } else {
        globals->assign(expr->name, value);
    }

    return value;
}


std::any Interpreter::visitCallExpr(Call* expr) {
    std::any calleeAny = evaluate(expr->callee.get());

    std::shared_ptr<LoxCallable> function;
    try {
        function =
            std::any_cast<std::shared_ptr<LoxCallable>>(calleeAny);
    } catch (...) {
        throw RuntimeError(expr->paren,
            "Can only call functions and classes.");
    }

    std::vector<std::any> arguments;
    for (const auto& arg : expr->arguments) {
        arguments.push_back(evaluate(arg.get()));
    }

    if (arguments.size() != function->arity()) {
        throw RuntimeError(
            expr->paren,
            "Expected " + std::to_string(function->arity()) +
            " arguments but got " +
            std::to_string(arguments.size())
        );
    }

    return function->call(*this, arguments);
}

/* =======================
   Statement execution
   ======================= */

void Interpreter::execute(Stmt* stmt) {
    stmt->accept(*this);
}

void Interpreter::resolve(Expr* expr, int depth) {
    locals[expr] = depth;
}

std::any Interpreter::visitExpressionStmt(Expression* stmt) {
    evaluate(stmt->expr.get());
    return {};
}

std::any Interpreter::visitPrintStmt(Print* stmt) {
    std::any value = evaluate(stmt->expr.get());
    std::cout << stringify(value) << "\n";
    return {};
}

std::any Interpreter::visitVarStmt(Var* stmt) {
    std::any value;
    if (stmt->initializer) {
        value = evaluate(stmt->initializer.get());
    }
    environment->define(stmt->name.lexeme, value);
    return {};
}

std::any Interpreter::visitBlockStmt(Block* stmt) {
    executeBlock(
        stmt->statements,
        std::make_shared<Environment>(environment)
    );
    return {};
}

std::any Interpreter::visitIfStmt(If* stmt) {
    if (isTruthy(evaluate(stmt->condition.get()))) {
        execute(stmt->then_branch.get());
    } else if (stmt->else_branch) {
        execute(stmt->else_branch.get());
    }
    return {};
}

std::any Interpreter::visitWhileStmt(While* stmt) {
    while (isTruthy(evaluate(stmt->condition.get()))) {
        execute(stmt->body.get());
    }
    return {};
}

std::any Interpreter::visitFunctionStmt(Function* stmt) {
    auto function =
        std::make_shared<LoxFunction>(stmt, environment);

    environment->define(
        stmt->name.lexeme,
        std::make_any<std::shared_ptr<LoxCallable>>(function)
    );

    return {};
}

std::any Interpreter::visitReturnStmt(Return* stmt) {
    std::any value;
    if (stmt->value) {
        value = evaluate(stmt->value.get());
    }
    throw FunctionReturn(value);
}

std::any Interpreter::lookUpVariable(const Token& name, Expr* expr) {
    auto it = locals.find(expr);
    if (it != locals.end()) {
        int distance = it->second;
        return environment->getAt(distance, name.lexeme);
    } else {
        return globals->get(name);
    }
}


/* =======================
   Utilities
   ======================= */

void Interpreter::executeBlock(
    const std::vector<std::unique_ptr<Stmt>>& statements,
    std::shared_ptr<Environment> newEnv
) {
    std::shared_ptr<Environment> previous = environment;
    environment = newEnv;

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

void Interpreter::checkNumberOperand(
    const Token& op,
    const std::any& operand
) {
    if (operand.type() == typeid(double)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(
    const Token& op,
    const std::any& left,
    const std::any& right
) {
    if (left.type() == typeid(double) &&
        right.type() == typeid(double)) return;
    throw RuntimeError(op, "Operands must be numbers.");
}

bool Interpreter::isTruthy(const std::any& value) {
    if (!value.has_value()) return false;
    if (value.type() == typeid(bool)) {
        return std::any_cast<bool>(value);
    }
    return true;
}

bool Interpreter::isEqual(
    const std::any& a,
    const std::any& b
) {
    if (!a.has_value() && !b.has_value()) return true;
    if (!a.has_value() || !b.has_value()) return false;
    if (a.type() != b.type()) return false;

    if (a.type() == typeid(double))
        return std::any_cast<double>(a) ==
               std::any_cast<double>(b);

    if (a.type() == typeid(std::string))
        return std::any_cast<std::string>(a) ==
               std::any_cast<std::string>(b);

    if (a.type() == typeid(bool))
        return std::any_cast<bool>(a) ==
               std::any_cast<bool>(b);

    return false;
}

std::string Interpreter::stringify(const std::any& object) {
    if (!object.has_value()) return "nil";

    if (object.type() == typeid(double)) {
        std::string text =
            std::to_string(std::any_cast<double>(object));
        if (text.size() >= 2 &&
            text.compare(text.size() - 2, 2, ".0") == 0) {
            text.erase(text.size() - 2);
        }
        return text;
    }

    if (object.type() == typeid(std::string))
        return std::any_cast<std::string>(object);

    if (object.type() == typeid(bool))
        return std::any_cast<bool>(object) ? "true" : "false";

    return "null";
}
