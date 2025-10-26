#include <vector>

#include "lox.h"
#include "scanner.h"
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "ast_printer.h"

namespace lox {

bool hadError = false;

void runFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open file: " << path << "\n";
        exit(66);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf(); // read the whole file
    run(buffer.str());      // pass contents to interpreter
}

void runPrompt() {
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break; // EOF or input error
        }
        run(line);  // pass each line to interpreter
    }
}

void run(const std::string& source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();
    Parser parser = Parser(tokens);
    std::unique_ptr<Expr> expression = parser.parse();

    if (hadError) return;
    if (expression) {
        std::cout << AstPrinter().print(*expression) << "\n";
    }

    // For now just print the tokens
    // for (const auto& token : tokens) {
    //     std::cout << token.toString() << "\n";
    //}


}

void error(int line, const std::string& message) {
    report(line, "", message);
}

void error(const Token& token, const std::string& message) {
    if (token.type == TokenType::END_OF_FILE) {
        report(token.line, " at end", message);
    } else {
        report(token.line, " at '" + token.lexeme + "'", message);
    }
}

void report(int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}

}