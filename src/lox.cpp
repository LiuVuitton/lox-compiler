#include "lox.h"
#include "token.h"
#include "scanner.h"
#include "token_type.h"
#include "expr.h"
#include "parser.h"
#include "ast_printer.h"
#include "runtime_error.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

namespace Lox {

bool had_error = false;
bool had_runtime_error = false;

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

    if (had_error) {
        exit(65);
    }
    if (had_runtime_error) {
        exit(70);
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
    interpreter.interpret(expression.get());

    std::cout << AstPrinter().print(expression.get()) << "\n";
}

void error(int line, const std::string& message) {
    report(line, "", message);
}

void report(int line, const std::string& where, const std::string& message) {
    std::cout << "[line " << line << "] Error" << where << ": " << message << "\n";
    had_error = true; 
}

void error(Token token, const std::string& message) {
    if (token.type == TokenType::END_OF_FILE) {
        report(token.line, " at end", message);
    }
    else {
        report(token.line, " at " + token.lexeme + " ", message);
    }
}

void runtimeError(const RuntimeError& error) {
    std::cout << error.what() << " \n[line " << error.token.line << "]\n";
    had_runtime_error = true;
}

} // namespace Lox
