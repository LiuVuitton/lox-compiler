#include "lox_callable.h"

NativeFunction::NativeFunction(std::function<std::any(Interpreter&, const std::vector<std::any>&)> func,
                               int arity)
    : func_(std::move(func)), arity_(arity) {}

int NativeFunction::arity() const {
    return arity_;
}

std::any NativeFunction::call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    return func_(interpreter, arguments);
}

std::string NativeFunction::toString() const {
    return "<native fn>";
}
