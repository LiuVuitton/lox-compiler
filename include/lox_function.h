#ifndef LOX_FUNCTION_H
#define LOX_FUNCTION_H

#include <memory>
#include <vector>
#include <any>
#include "lox_callable.h"
#include "stmt.h"
#include "environment.h"
#include "interpreter.h"

class LoxFunction : public LoxCallable {
public:
    LoxFunction(std::shared_ptr<Function> declaration);

    int arity() const override;

    std::any call(Interpreter& interpreter,
                  const std::vector<std::any>& arguments) override;

    std::string toString() const override;

private:
    std::shared_ptr<Function> declaration;
};

#endif // LOX_FUNCTION_H
