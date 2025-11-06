#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include <string>

//
// Step 1: Define all token names in a single macro list
//
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
    X(FALSE_) \
    X(FUN) \
    X(FOR) \
    X(IF) \
    X(NIL) \
    X(OR) \
    X(PRINT) \
    X(RETURN) \
    X(SUPER) \
    X(THIS) \
    X(TRUE_) \
    X(VAR) \
    X(WHILE) \
    X(END_OF_FILE)

//
// Step 2: Define the enum itself
//
enum class TokenType {
#define X(name) name,
    TOKEN_TYPE_LIST
#undef X
};

//
// Step 3: Declare the to_string function
//
std::string to_string(TokenType type);

#endif // TOKEN_TYPE_H
