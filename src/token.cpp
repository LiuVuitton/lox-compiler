#include "token.h"
#include "token_type.h"
#include <sstream>
#include <iostream>
#include <typeinfo>

namespace lox {

Token::Token(TokenType type, std::string lexeme, std::any literal, int line)
    : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}

std::string Token::toString() const {
    std::ostringstream out;
    out << tokenTypeToString(type) << " " << lexeme << " ";
    // Print literal depending on type stored in std::any
    if (!literal.has_value()) {
        out << "null";
    } else if (literal.type() == typeid(double)) {
        out << std::any_cast<double>(literal);
    } else if (literal.type() == typeid(std::string)) {
        out << std::any_cast<std::string>(literal);
    } else if (literal.type() == typeid(bool)) {
        out << (std::any_cast<bool>(literal) ? "true" : "false");
    } else {
        out << "<UNKNOWN>";
    }

    return out.str();
}

}