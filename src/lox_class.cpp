#include "lox_class.h"
#include "lox_instance.h"
#include <iostream>

LoxClass::LoxClass(const std::string& name) 
    : name(std::move(name)) {}

std::string LoxClass::toString() const {
    return name;
}

std::any LoxClass::call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    auto instance = std::make_shared<LoxInstance>(shared_from_this());
    return std::make_any<std::shared_ptr<LoxInstance>>(instance);
}

int LoxClass::arity() const {
    return 0;
}
