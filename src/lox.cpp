#include "lox.h"
#include "token.h"
#include "scanner.h"
#include "token_type.h"
#include "expr.h"
#include "parser.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

void runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        exit(64);
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); // Read entire file into buffer
    std::string source = buffer.str();

    run(source);

    if (Lox::had_error) {
        exit(65);
    }
}

void runPrompt() {
    std::string line;
    for (;;) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break; // EOF (Ctrl+D)
        run(line);
        Lox::had_error = false;
    }
}

void run(const std::string& source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens);
    std::unique_ptr<Expr> expression = parser.parse();

    // For now just print the tokens
    for (Token token : tokens) {
        std::cout << token.toString() << "\n";
    }

    // Stop if there was a syntax error
    if (had_error) return;
}

void error(int line, const std::string& message) {
    report(line, "", message);
}

void report(int line, const std::string& where, const std::string& message) {
    std::cout << "[line " << line << "] Error" << where << ": " << message << "\n";
    Lox::had_error = true; 
}

void error(Token token, const std::string& message) {
    if (token.type == TokenType::END_OF_FILE) {
        report(token.line, " at end", message);
    }
    else {
        report(token.line, " at " + token.lexeme + " ", message);
    }
}
