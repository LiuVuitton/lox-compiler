#include "compiler.h"
#include <iostream>
#include <cstdlib>
 
const std::unordered_map<TokenType, ParseRule> Compiler::rules = {
    { TokenType::LEFT_PAREN,    { &Compiler::grouping, nullptr, Precedence::NONE } },
    { TokenType::RIGHT_PAREN,   { nullptr, nullptr, Precedence::NONE } },
    { TokenType::LEFT_BRACE,    { nullptr, nullptr, Precedence::NONE } },
    { TokenType::RIGHT_BRACE,   { nullptr, nullptr, Precedence::NONE } },
    { TokenType::COMMA,         { nullptr, nullptr, Precedence::NONE } },
    { TokenType::DOT,           { nullptr, nullptr, Precedence::NONE } },

    { TokenType::MINUS,         { &Compiler::unary, &Compiler::binary, Precedence::TERM } },
    { TokenType::PLUS,          { nullptr, &Compiler::binary, Precedence::TERM } },
    { TokenType::SEMICOLON,     { nullptr, nullptr, Precedence::NONE } },

    { TokenType::SLASH,         { nullptr, &Compiler::binary, Precedence::FACTOR } },
    { TokenType::STAR,          { nullptr, &Compiler::binary, Precedence::FACTOR } },

    { TokenType::BANG,          { nullptr, nullptr, Precedence::NONE } },
    { TokenType::BANG_EQUAL,    { nullptr, nullptr, Precedence::NONE } },
    { TokenType::EQUAL,         { nullptr, nullptr, Precedence::NONE } },
    { TokenType::EQUAL_EQUAL,   { nullptr, nullptr, Precedence::NONE } },
    { TokenType::GREATER,       { nullptr, nullptr, Precedence::NONE } },
    { TokenType::GREATER_EQUAL, { nullptr, nullptr, Precedence::NONE } },
    { TokenType::LESS,          { nullptr, nullptr, Precedence::NONE } },
    { TokenType::LESS_EQUAL,    { nullptr, nullptr, Precedence::NONE } },

    { TokenType::IDENTIFIER,    { nullptr, nullptr, Precedence::NONE } },
    { TokenType::STRING,        { nullptr, nullptr, Precedence::NONE } },
    { TokenType::NUMBER,        { &Compiler::number, nullptr, Precedence::NONE } },

    { TokenType::AND,           { nullptr, nullptr, Precedence::NONE } },
    { TokenType::CLASS,         { nullptr, nullptr, Precedence::NONE } },
    { TokenType::ELSE,          { nullptr, nullptr, Precedence::NONE } },
    { TokenType::FALSE,         { nullptr, nullptr, Precedence::NONE } },
    { TokenType::FOR,           { nullptr, nullptr, Precedence::NONE } },
    { TokenType::FUN,           { nullptr, nullptr, Precedence::NONE } },
    { TokenType::IF,            { nullptr, nullptr, Precedence::NONE } },
    { TokenType::NIL,           { nullptr, nullptr, Precedence::NONE } },
    { TokenType::OR,            { nullptr, nullptr, Precedence::NONE } },
    { TokenType::PRINT,         { nullptr, nullptr, Precedence::NONE } },
    { TokenType::RETURN,        { nullptr, nullptr, Precedence::NONE } },
    { TokenType::SUPER,         { nullptr, nullptr, Precedence::NONE } },
    { TokenType::THIS,          { nullptr, nullptr, Precedence::NONE } },
    { TokenType::TRUE,          { nullptr, nullptr, Precedence::NONE } },
    { TokenType::VAR,           { nullptr, nullptr, Precedence::NONE } },
    { TokenType::WHILE,         { nullptr, nullptr, Precedence::NONE } },

    { TokenType::ERROR,         { nullptr, nullptr, Precedence::NONE } },
    { TokenType::END_OF_FILE,   { nullptr, nullptr, Precedence::NONE } },
};

Compiler::Compiler(const std::string& source)
    : scanner(source), parser(scanner), compiling_chunk(nullptr)
        {}

bool Compiler::compile(Chunk& chunk) {
    compiling_chunk = &chunk;

    parser.advance();
    expression();
    parser.consume(TokenType::END_OF_FILE, "Expect end of expression.");

    endCompiler();
    return !parser.had_error;
}

void Compiler::emitByte(uint8_t byte) {
    compiling_chunk->write(byte, parser.previous.line);
}

uint8_t Compiler::makeConstant(const Value& value) {
    int index = compiling_chunk->addConstant(value);
    if (index > UINT8_MAX) {
        error("Too many constants in one chunk.");
        return 0;
    }
    return static_cast<uint8_t>(index);
}

void Compiler::emitConstant(const Value& value) {
    emitBytes(OpCode::CONSTANT, makeConstant(value));
}

void Compiler::emitReturn() {
    emitByte(OpCode::RETURN);
}

void Compiler::endCompiler() {
    emitReturn();
    #ifdef DEBUG_PRINT_CODE
    if (!parser.had_error) {
        disassembleChunk(currentChunk(), "code");
    }
    #endif
}

void Compiler::number() {
    double value = std::stod(std::string(parser.previous.lexeme));
    emitConstant(value);
}

void Compiler::unary() {
    TokenType opType = parser.previous.type;
    // Compile the operand
    parsePrecedence(Precedence::UNARY);

    switch (opType) {
        case TokenType::MINUS: emitByte(OpCode::NEGATE); break;
        default: break;
    }
}

void Compiler::binary() {
    TokenType op_type = parser.previous.type;
    ParseRule rule = getRule(op_type);
    parsePrecedence(static_cast<Precedence>(static_cast<int>(rule.precedence) + 1));

    switch (op_type) {
        case TokenType::PLUS: emitByte(OpCode::ADD); break;
        case TokenType::MINUS: emitByte(OpCode::SUBTRACT); break;
        case TokenType::STAR: emitByte(OpCode::MULTIPLY); break;
        case TokenType::SLASH: emitByte(OpCode::DIVIDE); break;
        default: break;
    }
}

void Compiler::grouping() {
    expression();
    parser.consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
}

void Compiler::expression() {
    parsePrecedence(Precedence::ASSIGNMENT);
}

void Compiler::parsePrecedence(Precedence precedence) {
    parser.advance();
    ParseRule rule = getRule(parser.previous.type);
    ParseFn prefix = rule.prefix;
    if (!prefix) {
        errorAt(parser.previous, "Expect expression.");
        return;
    }

    (this->*prefix)();

    while (precedence <= getRule(parser.current.type).precedence) {
        parser.advance();
        ParseFn infix = getRule(parser.previous.type).infix;
        (this->*infix)();
    }
}

ParseRule Compiler::getRule(TokenType type) {
    return rules.at(type);
}

void Compiler::errorAt(const Token& token, std::string_view msg) {
    std::cerr << "[line " << token.line << "] Error at '"
              << token.lexeme << "': " << msg << "\n";
    parser.had_error = true;
}

void Compiler::error(std::string_view msg) {
    std::cerr << "Error: " << msg << "\n";
    parser.had_error = true;
}

void Compiler::errorAtCurrent(std::string_view msg) {
    errorAt(parser.current, msg);
}
