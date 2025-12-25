#ifndef NATIVE_FUNCTION_H
#define NATIVE_FUNCTION_H

#include <any>
#include <vector>
#include <functional>
#include "lox_callable.h"
#include "interpreter.h"

class NativeFunction : public LoxCallable {
public:
    NativeFunction(std::function<std::any(Interpreter&, const std::vector<std::any>&)> func, int arity);

    int arity() const override;
    std::any call(Interpreter& interpreter, const std::vector<std::any>& arguments) override;
    std::string toString() const override;

private:
    std::function<std::any(Interpreter&, const std::vector<std::any>&)> function;
    int _arity;
};

#endif // NATIVE_FUNCTION_H
