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
    void push(const Value& value);
    Value pop();
    Value peek(int distance);
    bool isFalsey(const Value& value);
    
    template <typename Op>
    void binaryOp(Op op) {
        double b = asNumber(pop());
        double a = asNumber(pop());
        push(makeNumber(op(a, b)));
    }

    void resetStack();
    void runtimeError(const std::string& message);
};
