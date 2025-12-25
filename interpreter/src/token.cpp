#include "token.h"
#include "utility.h"

Token::Token(TokenType type, std::string lexeme, std::any literal, int line)
    : type(type),
      lexeme(std::move(lexeme)),
      literal(std::move(literal)),
      line(line) {}

std::string Token::toString() const {
    return tokenTypeToString(type) + " " + lexeme + " " + literalToString(literal);
}
