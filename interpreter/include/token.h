#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <any>

#include "token_type.h"


struct Token {
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;
    Token(TokenType type, std::string lexeme, std::any literal, int line);
    std::string toString() const;
};

#endif // TOKEN_H
