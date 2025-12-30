#pragma once

#include "token_type.h"
#include <string>

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};