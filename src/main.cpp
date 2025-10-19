#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // for exit()

#include "lox.h"

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: clox [script]" << "\n";
        exit(64);
    }
    else if (argc == 2) {
        lox::runFile(argv[1]);
    }
    else {
        lox::runPrompt();
    }

    return 0;
}