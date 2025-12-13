#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include <any>

class Interpreter : public Expr::Visitor {
public:
    void interpret(Expr* expr);
    std::any visitLiteralExpr(Literal* expr) override;
    std::any visitGroupingExpr(Grouping* expr) override;
    std::any visitUnaryExpr(Unary* expr) override;
    std::any visitBinaryExpr(Binary* expr) override;
    void checkNumberOperand(const Token& op, const std::any& operand);
    void checkNumberOperands(const Token& op, const std::any& left, const std::any& right);

private:
    std::any evaluate(Expr* expr);
    bool isTruthy(const std::any& object);
    bool isEqual(const std::any& a, const std::any& b);
    std::string stringify(const std::any& object);
};

#endif // INTERPRETER_H