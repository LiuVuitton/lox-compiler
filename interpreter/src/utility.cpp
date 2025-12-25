#include "utility.h"

std::string tokenTypeToString(const TokenType& type) {
    switch (type) {
#define X(name) case TokenType::name: return #name;
        TOKEN_TYPE_LIST
#undef X
        default: return "UNKNOWN";
    }
}

std::string literalToString(const std::any& literal) {
    if (!literal.has_value()) {
        return "nil";
    }

    if (literal.type() == typeid(double)) {
        return std::to_string(std::any_cast<double>(literal));
    }
    if (literal.type() == typeid(std::string)) {
        return std::any_cast<std::string>(literal);
    }
    if (literal.type() == typeid(bool)) {
        return std::any_cast<bool>(literal) ? "true" : "false";
    }

    return "<unknown>";
}