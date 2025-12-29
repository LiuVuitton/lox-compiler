#include "value_array.h"
#include <iostream>

void ValueArray::write(const Value& value) {
    values.push_back(value);
}

void ValueArray::free() {
    values.clear();
}

size_t ValueArray::size() const {
    return values.size();
}


void printValue(const Value& value) {
    std::cout << value;
}