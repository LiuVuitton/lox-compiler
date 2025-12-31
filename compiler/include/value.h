#pragma once

#include <variant>
#include <vector>

using BOOL = bool;
using NUMBER = double;
using NIL = std::monostate;
using Value = std::variant<NIL, BOOL, NUMBER>;

struct ValueArray {
    std::vector<Value> values;
    ValueArray() = default;
    void write(const Value& value);
    void free();
    size_t size() const;
};

bool valuesEqual(const Value& a, const Value& b);
void printValue(const Value& value);

Value makeNil();
Value makeBool(BOOL b);
Value makeNumber(NUMBER n);

bool isNil(const Value& v);
bool isBool(const Value& v);
bool isNumber(const Value& v);

BOOL asBool(const Value& v);
NUMBER asNumber(const Value& v);
