#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include "stmt.h"
#include "environment.h"
#include "lox_class.h"

#include <any>
#include <memory>
#include <vector>
#include <string>

class Interpreter : public Expr::Visitor, public Stmt::Visitor {
public:
    Interpreter();

    void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);
    void resolve(Expr* expr, int depth);

    // Expr visitors
    std::any visitLiteralExpr(Literal* expr) override;
    std::any visitLogicalExpr(Logical* expr) override;
    std::any visitGroupingExpr(Grouping* expr) override;
    std::any visitUnaryExpr(Unary* expr) override;
    std::any visitVariableExpr(Variable* expr) override;
    std::any visitBinaryExpr(Binary* expr) override;
    std::any visitCallExpr(Call* expr) override;
    std::any visitAssignExpr(Assign* expr) override;

    // Stmt visitors
    std::any visitExpressionStmt(Expression* stmt) override;
    std::any visitFunctionStmt(Function* stmt) override;
    std::any visitIfStmt(If* stmt) override;
    std::any visitPrintStmt(Print* stmt) override;
    std::any visitReturnStmt(Return* stmt) override;
    std::any visitVarStmt(Var* stmt) override;
    std::any visitWhileStmt(While* stmt) override;
    std::any visitBlockStmt(Block* stmt) override;
    std::any visitClassStmt(Class* stmt) override;

    std::any lookUpVariable(const Token& name, Expr* expr);

    // Globals
    std::shared_ptr<Environment> globals;

    // Block execution helper
    void executeBlock(
        const std::vector<std::unique_ptr<Stmt>>& statements,
        std::shared_ptr<Environment> environment
    );

private:
    std::shared_ptr<Environment> environment;
    std::unordered_map<Expr*, int> locals;

    // Helpers
    std::any evaluate(Expr* expr);
    void execute(Stmt* stmt);

    bool isTruthy(const std::any& value);
    bool isEqual(const std::any& a, const std::any& b);
    std::string stringify(const std::any& object);

    void checkNumberOperand(const Token& op, const std::any& operand);
    void checkNumberOperands(const Token& op,
                             const std::any& left,
                             const std::any& right);
};

#endif // INTERPRETER_H
