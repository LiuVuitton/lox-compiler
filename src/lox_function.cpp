#include "lox_function.h"

LoxFunction::LoxFunction(std::shared_ptr<Function> declaration)
    : declaration(std::move(declaration)) {}

int LoxFunction::arity() const {
    return declaration->params.size();
}

std::any LoxFunction::call(Interpreter& interpreter,
                           const std::vector<std::any>& arguments) {
    auto environment = std::make_shared<Environment>(interpreter.globals);

    for (size_t i = 0; i < declaration->params.size(); ++i) {
        environment->define(declaration->params[i].lexeme, arguments[i]);
    }

    interpreter.executeBlock(declaration->body, environment);

    return std::any{};
}

std::string LoxFunction::toString() const {
    return "<fn " + declaration->name.lexeme + ">";
}
