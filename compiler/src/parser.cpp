#include "parser.h"
#include <iostream>

Parser::Parser(Scanner& scanner_)
    : scanner(scanner_) 
{
    // advance();
}

void Parser::advance() {
    previous = current;

    while (true) {
        current = scanner.scanToken();
        if (current.type != TokenType::ERROR) break;

        std::cerr << "Scanner error: " << current.lexeme << " at line " << current.line << "\n";
        had_error = true;
    }
}

bool Parser::check(TokenType type) const {
    return current.type == type;
}

bool Parser::match(TokenType type) {
    if (!check(type)) return false;
    advance();
    return true;
}

void Parser::consume(TokenType type, const std::string& msg) {
    if (check(type)) {
        advance();
        return;
    }

    std::cerr << "Parse error at line " << current.line << ": " << msg << "\n";
    had_error = true;
    panic_mode = true;
}

TokenType Parser::peekType() const {
    return current.type;
}
