#ifndef LOX_CALLABLE_H
#define LOX_CALLABLE_H

#include <any>
#include <vector>
#include <string>

class Interpreter;

class LoxCallable {
public:
    virtual ~LoxCallable() = default;
    virtual int arity() const = 0;
    virtual std::any call(Interpreter& interpreter, const std::vector<std::any>& arguments) = 0;
    virtual std::string toString() const = 0;
};

#endif // LOX_CALLABLE_H
