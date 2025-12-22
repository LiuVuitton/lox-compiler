#include "lox_class.h"
#include "lox_instance.h"
#include "lox_function.h"

LoxClass::LoxClass(
    const std::string& name,
    std::shared_ptr<LoxClass> superclass,
    std::unordered_map<std::string, std::shared_ptr<LoxCallable>> methods    
) 
    : name(std::move(name)), superclass(superclass), methods(std::move(methods)) {}

std::shared_ptr<LoxFunction> LoxClass::findMethod(const std::string& name) const {
    auto it = methods.find(name);
    if (it != methods.end()) {
        return std::dynamic_pointer_cast<LoxFunction>(it->second);
    }
    if (superclass) {
        return superclass->findMethod(name);
    }
    return nullptr;
}


std::string LoxClass::toString() const {
    return name;
}

std::any LoxClass::call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    auto instance = std::make_shared<LoxInstance>(shared_from_this());
    auto initializer = findMethod("this");
    if (initializer) {
        initializer->bind(instance)->call(interpreter, arguments);
    }
    return std::make_any<std::shared_ptr<LoxInstance>>(instance);
}

int LoxClass::arity() const {
    auto initializer = findMethod("init");
    if (initializer == nullptr) return 0;
    return initializer->arity();
}
