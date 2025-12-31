#pragma once

#include <variant>
#include <vector>
#include <memory>

struct Obj;

using BOOL = bool;
using NUMBER = double;
using NIL = std::monostate;
using OBJ = Obj;
using Value = std::variant<NIL, BOOL, NUMBER, std::shared_ptr<OBJ>>;

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

template<typename T, typename... Args>
Value makeObj(Args&&... args) {
    return Value{std::make_shared<T>(std::forward<Args>(args)...)}; // double check the correctness here
}


bool isNil(const Value& v);
bool isBool(const Value& v);
bool isNumber(const Value& v);

template<typename T>
bool isObjType(const Value& value) {
    if (!std::holds_alternative<std::shared_ptr<OBJ>>(value)) return false;
    auto& objPtr = std::get<std::shared_ptr<OBJ>>(value);
    return dynamic_cast<T*>(objPtr.get()) != nullptr;
}

BOOL asBool(const Value& v);
NUMBER asNumber(const Value& v);

template<typename T>
T* asObj(const Value& value) {
    auto& objPtr = std::get<std::shared_ptr<OBJ>>(value);
    return dynamic_cast<T*>(objPtr.get());
}