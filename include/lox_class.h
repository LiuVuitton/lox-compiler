#ifndef LOX_CLASS_H
#define LOX_CLASS_H

#include <string>
#include <vector>
#include <any>
#include <memory>
#include <unordered_map>
#include "lox_callable.h"

// Forward declaration
class LoxFunction;

class LoxClass : public LoxCallable, public std::enable_shared_from_this<LoxClass> {
public:
    explicit LoxClass(
        const std::string& name,
        std::unordered_map<std::string, std::shared_ptr<LoxCallable>> methods
    );
    std::shared_ptr<LoxFunction> findMethod(const std::string& name) const;

    std::any call(Interpreter& interpreter, const std::vector<std::any>& arguments) override;
    int arity() const override;
    std::string toString() const override;

private:
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<LoxCallable>> methods;
};

#endif // LOX_CLASS_H
