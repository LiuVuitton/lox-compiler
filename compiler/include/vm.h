#pragma once

#include "chunk.h"
#include <memory>
#include <vector>


enum class InterpretResult {
    OK,
    COMPILE_ERROR,
    RUNTIME_ERROR
};

#pragma once
#include "chunk.h"
#include <cstddef>

class VM {
    Chunk* chunk{nullptr};  // points to the currently running chunk
    size_t ip{0};           // index into chunk->code
    std::vector<Value> stack;

public:
    VM() = default;
    InterpretResult run();
    InterpretResult interpret(const std::string& source);

private:
    uint8_t readByte();
    const Value& readConstant();
    void resetStack();
    void push(const Value& value);
    Value pop();
    
    template <typename Op>
    void binaryOp(Op op) {
        double b = pop();
        double a = pop();
        push(Value{op(a, b)});
    }
};
