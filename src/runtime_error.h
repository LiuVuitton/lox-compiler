#ifndef RUNTIME_ERROR_H
#define RUNTIME_ERROR_H

#include <stdexcept>
#include "token.h"

namespace lox {

class RuntimeError : public std::runtime_error {
public:
    Token token;
    RuntimeError(const Token& token, const std::string& message)
        : std::runtime_error(message), token(token) {}
};

}

#endif