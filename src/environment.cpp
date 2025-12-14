#include "environment.h"
#include "runtime_error.h"

std::any Environment::get(const Token& name) {
    if (values.count(name.lexeme)) {
        return values[name.lexeme];
    }
    throw RuntimeError(name,
        "Undefined variable '" + name.lexeme + "'."
    );
}

void Environment::define(std::string name, std::any value) {
    values[name] = value;
}
