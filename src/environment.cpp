#include "environment.h"
#include "runtime_error.h"

std::any Environment::get(const Token& name) {
    auto it = values.find(name.lexeme);
    if (it != values.end()) {
        return it->second;
    }
    throw RuntimeError(name,
        "Undefined variable '" + name.lexeme + "'."  
    );
}


void Environment::define(const std::string& name, std::any value) {
    values[name] = value;
}

void Environment::assign(const Token& name, std::any value) {
    auto it = values.find(name.lexeme);
    if (it != values.end()) {
        it->second = std::move(value);
        return;
    }

    throw RuntimeError(name,
        "Undefined variable '" + name.lexeme + "'."
    );
}
