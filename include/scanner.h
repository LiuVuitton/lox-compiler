#ifndef SCANNER_H
#define SCANNER_H

#include <any>
#include <string>
#include <vector>
#include <unordered_map>

#include "token.h"

/**
 * @class Scanner
 * @brief Converts a source code string into a sequence of tokens.
 * 
 * The Scanner class implements lexical analysis for a Lox-like language.
 * It reads characters from the input source string, identifies lexemes,
 * and produces a vector of Token objects for the parser to consume.
 */

class Scanner {
public:
    Scanner(std::string source);
    std::vector<Token> scanTokens();

private:
    std::string source;
    std::vector<Token> tokens{};
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

    /// Returns true if the scanner has reached the end of the source.
    bool isAtEnd();

    /// Scans a single token and adds it to the tokens list.
    void scanToken();

    /// Advances the scanner by one character and returns it.
    char advance();

    /// Adds a token with the given type and no literal value.
    void addToken(TokenType type);

    /// Adds a token with the given type and literal value.
    void addToken(TokenType type, std::any literal);

    /// Consumes the next character if it matches the expected value.
    bool match(char expected);

    /// Peeks at the current character without consuming it.
    char peek();

    /// Parses a string literal.
    void string();

    /// Returns true if the character is a digit.
    bool isDigit(char c);

    /// Parses a numeric literal.
    void number();

    /// Peeks two characters ahead without consuming them.
    char peekNext();

    /// Parses an identifier or keyword.
    void identifier();

    /// Returns true if the character is an alphabetic character or underscore.
    bool isAlpha(char c);

    /// Returns true if the character is alphanumeric or underscore.
    bool isAlphaNumeric(char c);
};

#endif // SCANNER_H