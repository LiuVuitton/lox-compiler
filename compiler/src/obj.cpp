#include "obj.h"
#include <iostream>

void Obj::print() const {
    std::cout << asString();
}

void printObject(const Value& value) {
    auto& obj = std::get<std::shared_ptr<OBJ>>(value);
    obj->print();
}

bool Obj::isString() const {
    return false;
}

ObjString::ObjString(std::string lexeme) 
    : value(std::move(lexeme)) {}


std::string ObjString::asString() const {
    return value;
}

bool ObjString::equals(const Obj& other) const {
    auto* o = dynamic_cast<const ObjString*>(&other);
    if (!o) return false;
    return value == o->value;
}