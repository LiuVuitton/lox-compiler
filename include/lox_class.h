#ifndef LOX_CLASS_H
#define LOX_CLASS_H

#include <string>
#include <vector>
#include <any>
#include <memory>
#include "lox_callable.h"

class LoxClass : public LoxCallable, public std::enable_shared_from_this<LoxClass> {
public:
    explicit LoxClass(const std::string& name);

    std::any call(Interpreter& interpreter, const std::vector<std::any>& arguments) override;
    int arity() const override;
    std::string toString() const override;

private:
    std::string name;
};

#endif // LOX_CLASS_H
