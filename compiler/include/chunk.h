#pragma once
#include <cstdint>
#include <vector>
#include "value_array.h"

enum class OpCode {
    CONSTANT,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    NEGATE,
    RETURN,
};

constexpr uint8_t toByte(OpCode op);

struct Chunk {
    std::vector<uint8_t> code;
    std::vector<int> lines;
    ValueArray constants;

    Chunk() = default;
    void write(uint8_t byte, int line);
    void write(OpCode bytecode, int line);
    int addConstant(const Value& value);
    void free();
    size_t size() const;
};