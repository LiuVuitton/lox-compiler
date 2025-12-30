#include "chunk.h"
#include "debug.h"
#include "lox.h"
#include "vm.h"
#include <iostream>

int main(int argc, const char* argv[]) {
    VM vm{};
    
    if (argc == 1) {
        Lox::repl(vm);
    }
    else if (argc == 2) {
        Lox::runFile(vm, argv[1]);
    }
    else {
        std::cerr << "Usage: clox [path]\n";
        std::exit(64);
    }

    return 0;
}