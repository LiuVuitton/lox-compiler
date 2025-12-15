#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include "stmt.h"
#include <any>
#include <vector>
#include "environment.h"

class Interpreter : public Expr::Visitor, public Stmt::Visitor {
public:
    Interpreter();
    // void interpret(Expr* expr);
    void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);
    std::any visitLiteralExpr(Literal* expr) override;
    std::any visitGroupingExpr(Grouping* expr) override;
    std::any visitUnaryExpr(Unary* expr) override;
    std::any visitVariableExpr(Variable* expr) override;
    std::any visitBinaryExpr(Binary* expr) override;
    std::any visitExpressionStmt(Expression* stmt) override;
    std::any visitPrintStmt(Print* stmt) override;
    std::any visitVarStmt(Var* stmt) override;
    std::any visitAssignExpr(Assign* expr) override;
    std::any visitBlockStmt(Block* stmt) override;
    void checkNumberOperand(const Token& op, const std::any& operand);
    void checkNumberOperands(const Token& op, const std::any& left, const std::any& right);

private:
    std::shared_ptr<Environment> environment;
    std::any evaluate(Expr* expr);
    bool isTruthy(const std::any& object);
    bool isEqual(const std::any& a, const std::any& b);
    std::string stringify(const std::any& object);
    void execute(Stmt* stmt);
    void executeBlock(
        const std::vector<std::unique_ptr<Stmt>>& statements,
        std::shared_ptr<Environment> environment
    );
};

#endif // INTERPRETER_H