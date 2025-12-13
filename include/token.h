#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <any>

#include "token_type.h"


struct Token {
    const TokenType type;
    const std::string lexeme;
    const std::any literal;
    const int line;
    Token(TokenType type, std::string lexeme, std::any literal, int line);
    std::string toString() const;
};

#endif // TOKEN_H
