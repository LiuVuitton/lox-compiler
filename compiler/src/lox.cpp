#include "lox.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace Lox {

void repl(VM& vm) {
    std::string line;

    for (;;) {
        std::cout << "> " << std::flush;

        if (!std::getline(std::cin, line)) {
            std::cout << '\n';
            break;
        }

        vm.interpret(line);
    }
}

int runFile(VM& vm, const std::string& path) {
    const std::string source = readFile(path);
    const InterpretResult result = vm.interpret(source);

    switch (result) {
        case InterpretResult::OK:
            return 0;
        case InterpretResult::COMPILE_ERROR:
            return 65;
        case InterpretResult::RUNTIME_ERROR:
            return 70;
    }

    return 0;
}

std::string readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file \"" + path + "\".");
    }

    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

} // namespace Lox
