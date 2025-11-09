#include "lox.h"
#include "token.h"
#include "scanner.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void runFile(std::string path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        exit(64); // Exit code like in the book
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); // Read entire file into buffer
    std::string source = buffer.str();

    run(source);

    if (had_error) {
        exit(65);
    }
}

void runPrompt() {
    std::string line;
    for (;;) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break; // EOF (Ctrl+D)
        run(line);
        had_error = false;
    }
}

void run(std::string source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    // For now just print the tokens
    for (Token token : tokens) {
        std::cout << token.toString() << "\n";
    }
}

void error(int line, std::string message) {
    report(line, "", message);
}

void report(int line, std::string where, std::string message) {
    std::cout << "[line " << line << "] Error" << where << ": " << message << "\n";
    had_error = true; 
}