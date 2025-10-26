#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>
#include <string>
#include <variant>
#include "expr.h"
#include "token.h"

namespace lox {

class Interpreter : public Expr::Visitor {
public:
    using Value = std::variant<std::monostate, double, std::string, bool>;

    void interpret(std::unique_ptr<Expr>& expression);

    // Visitor methods for expressions
    std::any visitBinaryExpr(Binary* expr) override;
    std::any visitGroupingExpr(Grouping* expr) override;
    std::any visitLiteralExpr(Literal* expr) override;
    std::any visitUnaryExpr(Unary* expr) override;

private:
    bool isTruthy(const Value& value);
    bool isEqual(const Value& a, const Value& b);
    void checkNumberOperand(const Token& op, const Value& operand);
    void checkNumberOperands(const Token& op, const Value& left, const Value& right);
    std::string stringify(const Value& value);
};

} // namespace lox

#endif