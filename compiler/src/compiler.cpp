#include "compiler.h"
#include <iostream>
#include <cstdlib>
#include "obj.h"
#include "value.h"
 
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

    { TokenType::BANG,          { &Compiler::unary, nullptr, Precedence::NONE } },
    { TokenType::BANG_EQUAL,    { nullptr, &Compiler::binary, Precedence::EQUALITY } },
    { TokenType::EQUAL,         { nullptr, nullptr, Precedence::NONE } },
    { TokenType::EQUAL_EQUAL,   { nullptr, &Compiler::binary, Precedence::EQUALITY } },
    { TokenType::GREATER,       { nullptr, &Compiler::binary, Precedence::COMPARISON } },
    { TokenType::GREATER_EQUAL, { nullptr, &Compiler::binary, Precedence::COMPARISON } },
    { TokenType::LESS,          { nullptr, &Compiler::binary, Precedence::COMPARISON } },
    { TokenType::LESS_EQUAL,    { nullptr, &Compiler::binary, Precedence::COMPARISON } },

    { TokenType::IDENTIFIER,    { nullptr, nullptr, Precedence::NONE } },
    { TokenType::STRING,        { &Compiler::string, nullptr, Precedence::NONE } },
    { TokenType::NUMBER,        { &Compiler::number, nullptr, Precedence::NONE } },

    { TokenType::AND,           { nullptr, nullptr, Precedence::NONE } },
    { TokenType::CLASS,         { nullptr, nullptr, Precedence::NONE } },
    { TokenType::ELSE,          { nullptr, nullptr, Precedence::NONE } },
    { TokenType::FALSE,         {&Compiler::literal, nullptr, Precedence::NONE} },
    { TokenType::FOR,           { nullptr, nullptr, Precedence::NONE } },
    { TokenType::FUN,           { nullptr, nullptr, Precedence::NONE } },
    { TokenType::IF,            { nullptr, nullptr, Precedence::NONE } },
    { TokenType::NIL,           { &Compiler::literal, nullptr, Precedence::NONE } },
    { TokenType::OR,            { nullptr, nullptr, Precedence::NONE } },
    { TokenType::PRINT,         { nullptr, nullptr, Precedence::NONE } },
    { TokenType::RETURN,        { nullptr, nullptr, Precedence::NONE } },
    { TokenType::SUPER,         { nullptr, nullptr, Precedence::NONE } },
    { TokenType::THIS,          { nullptr, nullptr, Precedence::NONE } },
    { TokenType::TRUE,          {&Compiler::literal, nullptr, Precedence::NONE} },
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
    emitConstant(Value(value));
}

void Compiler::string() {
    std::string raw = parser.previous.lexeme;
    if (raw.size() >= 2) {
        raw = raw.substr(1, raw.size() - 2);
    }

    emitConstant(makeObj<ObjString>(raw));
}


void Compiler::unary() {
    TokenType opType = parser.previous.type;
    // Compile the operand
    parsePrecedence(Precedence::UNARY);

    switch (opType) {
        case TokenType::BANG:   emitByte(OpCode::NOT); break;
        case TokenType::MINUS:  emitByte(OpCode::NEGATE); break;
        default: break;
    }
}

void Compiler::binary() {
    TokenType op_type = parser.previous.type;
    ParseRule rule = getRule(op_type);
    parsePrecedence(static_cast<Precedence>(static_cast<int>(rule.precedence) + 1));

    switch (op_type) {
        case TokenType::BANG_EQUAL:     emitBytes(OpCode::EQUAL, OpCode::NOT); break;
        case TokenType::EQUAL_EQUAL:    emitByte(OpCode::EQUAL); break;
        case TokenType::GREATER:        emitByte(OpCode::GREATER); break;
        case TokenType::GREATER_EQUAL:  emitBytes(OpCode::LESS, OpCode::NOT); break;
        case TokenType::LESS:           emitByte(OpCode::LESS); break;
        case TokenType::LESS_EQUAL:     emitBytes(OpCode::GREATER, OpCode::NOT); break;
        case TokenType::PLUS:           emitByte(OpCode::ADD); break;
        case TokenType::MINUS:          emitByte(OpCode::SUBTRACT); break;
        case TokenType::STAR:           emitByte(OpCode::MULTIPLY); break;
        case TokenType::SLASH:          emitByte(OpCode::DIVIDE); break;
        default: break;
    }
}

void Compiler::literal() {
    switch (parser.previous.type) {
        case TokenType::FALSE:  emitByte(OpCode::FALSE); break;
        case TokenType::NIL:    emitByte(OpCode::NIL); break;
        case TokenType::TRUE:   emitByte(OpCode::TRUE); break;
        default:
            return;
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
