#include "lox_function.h"
#include "lox_instance.h"
#include "lox_class.h"
#include "runtime_error.h"


LoxInstance::LoxInstance(std::shared_ptr<LoxClass> klass)
    : klass(std::move(klass)) {}


std::any LoxInstance::get(const Token& name) {
    auto it = fields.find(name.lexeme);
    if (it != fields.end()) {
        return it->second;
    }

    auto method = klass->findMethod(name.lexeme);
    if (method) {
        return std::static_pointer_cast<LoxCallable>(method);
    }
    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}


void LoxInstance::set(const Token& name, std::any value) {
    fields[name.lexeme] = value;
}

std::string LoxInstance::toString() {
    return klass->toString() + " instance";
}