#pragma once

#include <vector>
#include "value.h"

struct ValueArray {
    std::vector<Value> values;
    ValueArray() = default;
    void write(const Value& value);
    void free();
    size_t size() const;
};


void printValue(const Value& value);