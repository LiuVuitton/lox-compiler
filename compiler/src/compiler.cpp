#include "compiler.h"


#include <iostream>
#include <format>
#include "scanner.h"
#include "token.h"

bool Compiler::compile(const std::string& source, const Chunk* chunk) {
    Scanner scanner(source);
    advance();
    expression();
    consume(TokenType::END_OF_FILE, "Expected end of expression.");
    endCompiler();
    return !parser.had_error;
}

Chunk* Compiler::currentChunk() {
    return compiling_chunk;
}

void Compiler::advance() {
    parser.previous = parser.current;

    while (true) {
        parser.current = scanner.scanToken();
        if (parser.current.type != TokenType::ERROR) break;
        errorAtCurrent(parser.current.lexeme);
    }
}

void Compiler::consume(TokenType type, std::string_view msg) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(msg);
}

void Compiler::emitByte(OpCode bytecode) {
    currentChunk()->write(static_cast<uint8_t>(bytecode), parser.previous.line);
}
    
void Compiler::emitBytes(OpCode bytecode1, OpCode bytecode2) {
    emitByte(bytecode1);
    emitByte(bytecode2);
}

void Compiler::emitReturn() {
    emitByte(OpCode::RETURN);
}

void Compiler::endCompiler() {
    emitReturn();
}

void Compiler::expression() {
    
}

void Compiler::errorAt(const Token& token, std::string_view msg) {
    if (parser.panic_mode) return;
    parser.panic_mode = true;

    std::cerr << "[line " << token.line << "] Error";

    switch (token.type) {
        case TokenType::END_OF_FILE:
            std::cerr << " at end";
            break;
        case TokenType::ERROR:
            break;
        default:
            std::cerr << " at '" << token.lexeme << "'";
            break;
    }

    std::cerr << ": " << msg << "\n";

    parser.had_error = true;
}


void Compiler::error(std::string_view msg) {
    errorAt(parser.previous, msg);
}

void Compiler::errorAtCurrent(std::string_view msg) {
    errorAt(parser.current, msg);
}