#pragma once

#include <string>
#include "value.h"

struct Obj {
    virtual ~Obj() = default;
    virtual std::string asString() const = 0;
    virtual void print() const;
    virtual bool equals(const Obj& other) const = 0;

    virtual bool isString() const;
};

void printObject(const Value& value);

struct ObjString : Obj {
    std::string value;

    explicit ObjString(std::string lexeme);
    std::string asString() const override;
    bool equals(const Obj& other) const override;
};