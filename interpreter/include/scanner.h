#ifndef SCANNER_H
#define SCANNER_H

#include <any>
#include <string>
#include <vector>
#include <unordered_map>

#include "token.h"

class Scanner {
public:
    Scanner(const std::string& source);
    std::vector<Token> scanTokens();

private:
    std::string source;
    std::vector<Token> tokens;
    int start{0};
    int current{0};
    int line{1};
    std::unordered_map<std::string, TokenType> keywords{
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"for", TokenType::FOR},
        {"fun", TokenType::FUN},
        {"if", TokenType::IF},
        {"nil", TokenType::NIL},
        {"or", TokenType::OR},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"var", TokenType::VAR},
        {"while", TokenType::WHILE}
    };

    bool isAtEnd();

    void scanToken();

    char advance();

    void addToken(TokenType type);

    void addToken(TokenType type, std::any literal);

    bool match(char expected);

    // Look ahead, similar to advance but doesn't consume the character
    char peek();

    void string();

    bool isDigit(char c);

    void number();

    char peekNext();

    void identifier();

    bool isAlpha(char c);

    bool isAlphaNumeric(char c);
};

#endif // SCANNER_H