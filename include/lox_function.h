#ifndef LOX_FUNCTION_H
#define LOX_FUNCTION_H

#include "lox_callable.h"
#include "stmt.h"
#include "environment.h"
#include "interpreter.h"
#include <memory>
#include <vector>
#include <string>


class LoxFunction : public LoxCallable {
public:
    LoxFunction(Function* declaration,
                std::shared_ptr<Environment> closure);

    int arity() const override;
    std::any call(Interpreter& interpreter,
                  const std::vector<std::any>& arguments) override;
    std::string toString() const override;

private:
    std::string name;
    std::vector<Token> params;
    std::vector<std::unique_ptr<Stmt>> body;
    std::shared_ptr<Environment> closure;
};

#endif // LOX_FUNCTION_H
