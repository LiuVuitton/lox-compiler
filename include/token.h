#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <any>

#include "token_type.h"

struct Token {
public:
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;
    Token(TokenType type, std::string lexeme, std::any literal, int line);
    std::string toString();
private:
};

#endif // TOKEN_H