#include <iostream>

#include "lox.h"

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::cout << "Usage: clox [script]\n";
        std::exit(64);
    }
    else if (argc == 1) {
        Lox::runFile(argv[0]);
    }
    else {
        Lox::runPrompt();
    }
}