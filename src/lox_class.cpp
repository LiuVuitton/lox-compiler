#include "lox_class.h"
#include "lox_instance.h"
#include "lox_function.h"

LoxClass::LoxClass(
    const std::string& name,
    std::unordered_map<std::string, std::shared_ptr<LoxCallable>> methods    
) 
    : name(std::move(name)), methods(std::move(methods)) {}

std::shared_ptr<LoxFunction> LoxClass::findMethod(const std::string& name) {
    if (methods.count(name)) {
        return std::dynamic_pointer_cast<LoxFunction>(methods[name]);
    }
    return nullptr;
}

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
