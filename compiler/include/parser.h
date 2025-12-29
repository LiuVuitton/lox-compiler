#pragma once

#include "token.h"

class Parser {

public:
    Token current;
    Token previous;
    bool had_error{false};
    bool panic_mode{false};
    Parser() = default;

private:
    void advance();

};
