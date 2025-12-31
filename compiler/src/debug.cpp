#include "debug.h"
#include <iostream>
#include <format>

void disassembleChunk(const Chunk &chunk, const std::string &name) {
    std::cout << "== " << name << " ==\n";

    for (int offset = 0; offset < chunk.size();) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int constantInstruction(const std::string& name, const Chunk& chunk, int offset) {
    uint8_t constant = chunk.code[offset + 1];

    std::cout << std::format(
        "{:<16} {:4} '",
        name,
        constant
    );

    printValue(chunk.constants.values[constant]);
    std::cout << "'\n";

    return offset + 2;
}

static int simpleInstruction(const std::string& name, int offset) {
    std::cout << name << "\n";
    return offset + 1;
}

int disassembleInstruction(const Chunk& chunk, int offset) {
    std::cout << std::format("{:04} ", offset);

    if (offset > 0 && chunk.lines[offset] == chunk.lines[offset - 1]) {
        std::cout << "   | ";
    } else {
        std::cout << std::format("{:4} ", chunk.lines[offset]);
    }

    OpCode instruction = static_cast<OpCode>(chunk.code[offset]);

    switch (instruction) {
        case OpCode::CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);

        case OpCode::NIL:
            return simpleInstruction("OP_NIL", offset);

        case OpCode::TRUE:
            return simpleInstruction("OP_TRUE", offset);

        case OpCode::FALSE:
            return simpleInstruction("OP_FALSE", offset);
        
        case OpCode::EQUAL:
            return simpleInstruction("OP_EQUAL", offset);

        case OpCode::GREATER:
            return simpleInstruction("OP_GREATER", offset);

        case OpCode::LESS:
            return simpleInstruction("OP_LESS", offset);

        case OpCode::ADD:
            return simpleInstruction("OP_ADD", offset);

        case OpCode::SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);

        case OpCode::MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);

        case OpCode::DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);

        case OpCode::NOT:
            return simpleInstruction("NOT", offset);

        case OpCode::NEGATE:
            return simpleInstruction("OP_NEGATE", offset);

        case OpCode::RETURN:
            return simpleInstruction("OP_RETURN", offset);

        default:
            std::cout << std::format(
                "Unknown opcode {}\n",
                static_cast<uint8_t>(instruction)
            );
            return offset + 1;
    }
}
