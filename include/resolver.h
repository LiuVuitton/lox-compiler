#ifndef RESOLVER_H
#define RESOLVER_H

#include "expr.h"
#include "stmt.h"
#include "interpreter.h"
#include <unordered_map>
#include <vector>

class Resolver : public Expr::Visitor, public Stmt::Visitor {
public:
    explicit Resolver(Interpreter& interpreter);
    void resolve(const std::vector<std::unique_ptr<Stmt>>& statements);

    // Stmt visitors
    std::any visitBlockStmt(Block* stmt) override;
    std::any visitClassStmt(Class* stmt) override;
    std::any visitExpressionStmt(Expression* stmt) override;
    std::any visitFunctionStmt(Function* stmt) override;
    std::any visitIfStmt(If* stmt) override;
    std::any visitPrintStmt(Print* stmt) override;
    std::any visitReturnStmt(Return* stmt) override;
    std::any visitVarStmt(Var* stmt) override;
    std::any visitWhileStmt(While* stmt) override;

    // Expr visitors
    std::any visitAssignExpr(Assign* expr) override;
    std::any visitBinaryExpr(Binary* expr) override;
    std::any visitCallExpr(Call* expr) override;
    std::any visitGetExpr(Get* expr) override;
    std::any visitGroupingExpr(Grouping* expr) override;
    std::any visitLiteralExpr(Literal* expr) override;
    std::any visitLogicalExpr(Logical* expr) override;
    std::any visitSetExpr(Set* expr) override;
    std::any visitSuperExpr(Super* expr) override;
    std::any visitThisExpr(This* expr) override;
    std::any visitUnaryExpr(Unary* expr) override;
    std::any visitVariableExpr(Variable* expr) override;

private:
    enum class FunctionType {
        NONE,
        FUNCTION,
        INITIALIZER,
        METHOD
    };

    enum class ClassType {
        NONE,
        CLASS,
        SUBCLASS
    };
    Interpreter& interpreter;
    std::vector<std::unordered_map<std::string, bool>> scopes;
    FunctionType current_function{FunctionType::NONE};
    ClassType current_class{ClassType::NONE};

    void resolve(Stmt* stmt);
    void resolve(Expr* expr);
    void resolveFunction(Function* function, FunctionType type);

    void beginScope();
    void endScope();
    void declare(const Token& name);
    void define(const Token& name);
    void resolveLocal(Expr* expr, const Token& name);
};

#endif // RESOLVER_H
