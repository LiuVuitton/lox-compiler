#include "value.h"
#include <iostream>
#include "obj.h"

void ValueArray::write(const Value& value) {
    values.push_back(value);
}

void ValueArray::free() {
    values.clear();
}

size_t ValueArray::size() const {
    return values.size();
}

bool valuesEqual(const Value& a, const Value& b) {
    if (std::holds_alternative<std::shared_ptr<OBJ>>(a)) {
        auto& objA = std::get<std::shared_ptr<OBJ>>(a);
        auto& objB = std::get<std::shared_ptr<OBJ>>(b);
        if (!objA || !objB) return false;
        return objA->equals(*objB);
    }
    return a == b; // This is possible because std::variant already implements == operator.
}


void printValue(const Value& value) {
    if (std::holds_alternative<std::monostate>(value))
        std::cout << "nil";
    else if (std::holds_alternative<bool>(value))
        std::cout << (std::get<bool>(value) ? "true" : "false");
    else if (std::holds_alternative<double>(value))
        std::cout << std::get<double>(value);
    else if (std::holds_alternative<std::shared_ptr<OBJ>>(value)) 
        printObject(value);
}

Value makeNil() {
    return NIL{};
}

Value makeBool(BOOL b) {
    return b;

}
Value makeNumber(NUMBER n) {
    return n;
}

bool isNil(const Value& v) {
    return std::holds_alternative<NIL>(v);
}

bool isBool(const Value& v) {
    return std::holds_alternative<BOOL>(v);
}

bool isNumber(const Value& v) {
    return std::holds_alternative<NUMBER>(v);
}

BOOL asBool(const Value& v) {
    return std::get<BOOL>(v);
}

NUMBER asNumber(const Value& v) {
    return std::get<NUMBER>(v);
}
