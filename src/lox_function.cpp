#include "lox_function.h"
#include "function_return.h"
#include "lox_instance.h"

LoxFunction::LoxFunction(Function* declaration,
                         std::shared_ptr<Environment> closure,
                         bool isInitializer)
    : name(declaration->name.lexeme),
      params(declaration->params),
      body(std::make_shared<std::vector<std::unique_ptr<Stmt>>>(std::move(declaration->body))),
      closure(closure),
      isInitializer(isInitializer) {}

LoxFunction::LoxFunction(const std::string& name,
                         const std::vector<Token>& params,
                         std::shared_ptr<std::vector<std::unique_ptr<Stmt>>> body,
                         std::shared_ptr<Environment> closure,
                         bool isInitializer)
    : name(name),
      params(params),
      body(body),
      closure(closure),
      isInitializer(isInitializer) {}

std::shared_ptr<LoxFunction> LoxFunction::bind(std::shared_ptr<LoxInstance> instance) {
    auto environment = std::make_shared<Environment>(closure);
    environment->define("this", std::make_any<std::shared_ptr<LoxInstance>>(instance));

    return std::make_shared<LoxFunction>(name, params, body, environment, isInitializer);
}

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
        interpreter.executeBlock(*body, environment);
    } catch (FunctionReturn returnValue) {
        if (isInitializer) return closure->getAt(0, "this");
        return returnValue.value;
    }

    if (isInitializer) return closure->getAt(0, "this");
    return std::any{};
}

std::string LoxFunction::toString() const {
    return "<fn " + name + ">";
}
