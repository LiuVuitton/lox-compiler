#include "lox_function.h"
#include "function_return.h"

LoxFunction::LoxFunction(Function* declaration,
                         std::shared_ptr<Environment> closure)
    : name(declaration->name.lexeme),
      params(declaration->params),
      body(std::move(declaration->body)),
      closure(closure) {}

int LoxFunction::arity() const {
    return static_cast<int>(params.size());
}

std::any LoxFunction::call(Interpreter& interpreter,
                           const std::vector<std::any>& arguments) {

    auto environment = std::make_shared<Environment>(closure);

    for (size_t i = 0; i < params.size(); ++i) {
        environment->define(params[i].lexeme, arguments[i]);
    }

    try {
        interpreter.executeBlock(body, environment);
    } catch (FunctionReturn returnValue) {
        return returnValue.value;
    }

    return std::any{};
}

std::string LoxFunction::toString() const {
    return "<fn " + name + ">";
}
