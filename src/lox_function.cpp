#include "lox_function.h"
#include "function_return.h"

LoxFunction::LoxFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure)
    : declaration(std::move(declaration)), closure(std::move(closure)) {}

int LoxFunction::arity() const {
    return declaration->params.size();
}

std::any LoxFunction::call(Interpreter& interpreter,
                           const std::vector<std::any>& arguments) {
    auto environment = std::make_shared<Environment>(closure);

    for (size_t i = 0; i < declaration->params.size(); ++i) {
        environment->define(declaration->params[i].lexeme, arguments[i]);
    }

    try {
        interpreter.executeBlock(declaration->body, environment);
    }
    catch (FunctionReturn returnValue) {
        return returnValue.value;
    }
        
    return std::any{};
}

std::string LoxFunction::toString() const {
    return "<fn " + declaration->name.lexeme + ">";
}
