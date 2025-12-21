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
    throw RuntimeError(name, "Undefined proprerty '" + name.lexeme + "'.");
}

std::string LoxInstance::toString() {
    return klass->toString() + " instance";
}