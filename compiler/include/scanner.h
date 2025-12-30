#pragma once
#include <string_view>
#include "token.h"

class Scanner {
    std::string source;
    int start = 0;
    int current = 0;
    int line = 1;

public:
    explicit Scanner(const std::string& source);

    Token scanToken();

private:
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAtEnd() const;
    Token makeToken(TokenType type) const;
    Token errorToken(const std::string& msg) const;
    void skipWhitespace();
    TokenType checkKeyword(const std::string& lexeme, std::size_t start,
                           const std::string& rest, TokenType type) const;
    TokenType identifierType() const;
    Token identifier();
    Token number();
    Token string();
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);
};
