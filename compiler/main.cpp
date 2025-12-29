#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
    VM vm{};
    Chunk chunk{};
    int constant = chunk.addConstant(1.2);
    chunk.write(OpCode::CONSTANT, 123);
    chunk.write(constant, 123);

    constant = chunk.addConstant(3.4);
    chunk.write(OpCode::CONSTANT, 123);
    chunk.write(constant, 123);

    chunk.write(OpCode::ADD, 123);

    constant = chunk.addConstant(5.6);
    chunk.write(OpCode::CONSTANT, 123);
    chunk.write(constant, 123);
    chunk.write(OpCode::DIVIDE, 123);

    chunk.write(OpCode::NEGATE, 123);
    chunk.write(OpCode::RETURN, 123);
    disassembleChunk(chunk, "test chunk");
    vm.interpret(&chunk);
    return 0;
}