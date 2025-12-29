#pragma once

#include <string>
#include "chunk.h"
#include "parser.h"
#include "scanner.h"

class Compiler {
    Scanner scanner;
    Parser parser;
    Chunk* compiling_chunk{nullptr};

public:
    Compiler() = default;
    bool compile(const std::string& source, const Chunk* chunk);

private:
    Chunk* currentChunk();
    void advance();
    void consume(TokenType type, std::string_view msg);
    void emitByte(OpCode bytecode);
    void emitBytes(OpCode bytecode1, OpCode bytecode2);
    void emitReturn();
    uint8_t makeConstant(); // TOOD
    void emitConstant(); // TODO
    void endCompiler();
    void number(); // TODO

    void expression();
    void errorAt(const Token& token, std::string_view msg);
    void error(std::string_view msg);
    void errorAtCurrent(std::string_view msg);
};
