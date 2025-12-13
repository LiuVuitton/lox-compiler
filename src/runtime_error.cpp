#include "runtime_error.h"

RuntimeError::RuntimeError(const Token& token, const std::string& message)
        : std::runtime_error(message), token(token) {}