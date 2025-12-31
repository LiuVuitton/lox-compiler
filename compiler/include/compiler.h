#pragma once
#include <string>
#include "chunk.h"
#include "parser.h"
#include "scanner.h"
#include <unordered_map>

class Compiler {
    Scanner scanner;
    Parser parser;
    Chunk* compiling_chunk{nullptr};
    static const std::unordered_map<TokenType, ParseRule> rules;

public:
    explicit Compiler(const std::string& source);

    bool compile(Chunk& chunk);

private:

    // Bytecode emission
    void emitByte(uint8_t byte);
    template <typename T>
    void emitByte(T byte) { emitByte(static_cast<uint8_t>(byte)); }
    template <typename T1, typename T2>
    void emitBytes(T1 byte1, T2 byte2) { emitByte(byte1); emitByte(byte2); }
    void emitReturn();
    uint8_t makeConstant(const Value& value);
    void emitConstant(const Value& value);
    void endCompiler();

    // Parsing functions (Compiler member functions)
    void number();
    void string();
    void unary();
    void binary();
    void literal();
    void grouping();
    void expression();
    void parsePrecedence(Precedence precedence);

    // Error handling
    void errorAt(const Token& token, std::string_view msg);
    void error(std::string_view msg);
    void errorAtCurrent(std::string_view msg);

    // Parse rules table
    static ParseRule getRule(TokenType type);
};
