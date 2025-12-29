#include "chunk.h"

constexpr uint8_t toByte(OpCode op) {
    return static_cast<uint8_t>(op);
}

void Chunk::write(uint8_t byte, int line) {
    code.push_back(byte);
    lines.push_back(line);
}

void Chunk::write(OpCode bytecode, int line) {
    uint8_t byte = static_cast<int>(bytecode);
    write(byte, line);
}

int Chunk::addConstant(const Value& value) {
    constants.write(value);
    return constants.size() - 1;
}

void Chunk::free() {
    code.clear();
    constants.free();
}

size_t Chunk::size() const {
    return code.size();
}