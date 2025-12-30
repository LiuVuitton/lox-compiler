#pragma once

#include <cstdint>

// X-macro list of all token types
#define TOKEN_TYPE_LIST \
    /* Single-character tokens */ \
    X(LEFT_PAREN) \
    X(RIGHT_PAREN) \
    X(LEFT_BRACE) \
    X(RIGHT_BRACE) \
    X(COMMA) \
    X(DOT) \
    X(MINUS) \
    X(PLUS) \
    X(SEMICOLON) \
    X(SLASH) \
    X(STAR) \
    /* One or two character tokens */ \
    X(BANG) \
    X(BANG_EQUAL) \
    X(EQUAL) \
    X(EQUAL_EQUAL) \
    X(GREATER) \
    X(GREATER_EQUAL) \
    X(LESS) \
    X(LESS_EQUAL) \
    /* Literals */ \
    X(IDENTIFIER) \
    X(STRING) \
    X(NUMBER) \
    /* Keywords */ \
    X(AND) \
    X(CLASS) \
    X(ELSE) \
    X(FALSE) \
    X(FUN) \
    X(FOR) \
    X(IF) \
    X(NIL) \
    X(OR) \
    X(PRINT) \
    X(RETURN) \
    X(SUPER) \
    X(THIS) \
    X(TRUE) \
    X(VAR) \
    X(WHILE) \
    /* Special */ \
    X(ERROR) \
    X(END_OF_FILE) \
    X(COUNT)

enum class TokenType : uint8_t {
#define X(name) name,
    TOKEN_TYPE_LIST
#undef X
};
