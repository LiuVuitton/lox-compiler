#ifndef LOX_CALLABLE_H
#define LOX_CALLABLE_H

#include <any>
#include <vector>
#include <functional>
#include <string>
#include <memory>
#include "interpreter.h"

class LoxCallable {
public:
    virtual ~LoxCallable() = default;

    virtual int arity() const = 0;

    virtual std::any call(Interpreter& interpreter, const std::vector<std::any>& arguments) = 0;

    virtual std::string toString() const { return "<fn>"; }
};

class NativeFunction : public LoxCallable {
public:
    NativeFunction(std::function<std::any(Interpreter&, const std::vector<std::any>&)> func,
                   int arity);

    int arity() const override;
    std::any call(Interpreter& interpreter, const std::vector<std::any>& arguments) override;
    std::string toString() const override;

private:
    std::function<std::any(Interpreter&, const std::vector<std::any>&)> func_;
    int arity_;
};

#endif
