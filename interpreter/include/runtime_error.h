#ifndef RUNTIME_ERROR_H
#define RUNTIME_ERROR_H

#include <stdexcept>
#include <string>
#include "token.h"

class RuntimeError : public std::runtime_error {
public:
    const Token token;

    RuntimeError(const Token& token, const std::string& message);
};

#endif // RUNTIME_ERROR_H