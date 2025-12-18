#include "native_function.h"
#include <string>

NativeFunction::NativeFunction(
    std::function<std::any(Interpreter&, const std::vector<std::any>&)> func,
    int arity)
    : function(func), _arity(arity) {}

int NativeFunction::arity() const {
    return _arity;
}

std::any NativeFunction::call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    return function(interpreter, arguments);
}

std::string NativeFunction::toString() const {
    return "<native fn>";
}
