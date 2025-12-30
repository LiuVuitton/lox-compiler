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
    void emitByte(uint8_t byte);

    template <typename T>
    void emitByte(T byte) {
        emitByte(static_cast<uint8_t>(byte));
    }

    template <typename T1, typename T2>
    void emitBytes(T1 byte1, T2 byte2) {
        emitByte(byte1); 
        emitByte(byte2);
    }

    void emitReturn();
    uint8_t makeConstant(const Value& value);
    void emitConstant(const Value& value);
    void endCompiler();
    void number();

    void expression();
    void errorAt(const Token& token, std::string_view msg);
    void error(std::string_view msg);
    void errorAtCurrent(std::string_view msg);
};
