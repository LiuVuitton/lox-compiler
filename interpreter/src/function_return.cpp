#include "function_return.h"

FunctionReturn::FunctionReturn(std::any value)
    : value(std::move(value)) {}