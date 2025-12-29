#pragma once

#include "token_type.h"
#include <string_view>

struct Token {
    TokenType type;
    std::string_view lexeme;
    int line;
};