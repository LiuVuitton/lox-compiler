#pragma once
#include <functional>
#include "token.h"
#include "scanner.h"


enum class Precedence {
    NONE,
    ASSIGNMENT,
    OR,
    AND,
    EQUALITY,
    COMPARISON,
    TERM,
    FACTOR,
    UNARY,
    CALL,
    PRIMARY
};

class Compiler;
using ParseFn = void (Compiler::*)();

struct ParseRule {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
};

class Parser {
    Scanner& scanner;

public:
    Token current;
    Token previous;
    bool had_error{false};
    bool panic_mode{false};

    explicit Parser(Scanner& scanner);

    void advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    void consume(TokenType type, const std::string& msg);
    TokenType peekType() const;
};
