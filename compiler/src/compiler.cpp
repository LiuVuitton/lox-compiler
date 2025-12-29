#include "compiler.h"


#include <iostream>
#include <format>
#include "scanner.h"
#include "token.h"

void compile(std::string_view source) {
    Scanner scanner(source);
    int line = -1;

    while (true) {
        Token token = scanner.scanToken();

        if (token.line != line) {
            std::cout << std::format("{:4} ", token.line);
            line = token.line;
        } else {
            std::cout << " | ";
        }

        std::cout << std::format("{:2} '{}'\n",
                                 static_cast<int>(token.type),
                                 token.lexeme);

        if (token.type == TokenType::EOF) break;
    }
}

