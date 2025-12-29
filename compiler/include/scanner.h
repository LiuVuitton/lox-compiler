#pragma once

#include <string_view>
#include "token.h"

class Scanner {
    std::string_view source;
    int start = 0;
    int current = 0;
    int line = 1;
public:
    explicit Scanner(std::string_view source);

    Token scanToken();

private:
    bool isAlpha(char c);
    bool isDigit(char c);
    bool isAtEnd() const;
    Token makeToken(TokenType type) const;
    Token errorToken(std::string_view msg) const;
    void skipWhitespace();
    TokenType checkKeyword(std::string_view lexeme, std::size_t start,
                           std::string_view rest, TokenType type);
    TokenType identifierType();
    Token identifier();
    Token number();
    Token string();
    char advance();
    char peek();
    char peekNext();
    bool match(char expected);
};
