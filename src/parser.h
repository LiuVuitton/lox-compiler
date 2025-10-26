#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include <stdexcept>
#include "token.h"
#include "expr.h"

namespace lox {

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    std::unique_ptr<Expr> parse();

private:
    // Custom ParseError exception (like Java’s inner class)
    class ParseError : public std::runtime_error {
    public:
        explicit ParseError(const std::string& message)
            : std::runtime_error(message) {}
    };

    // Grammar methods
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    // Utility methods
    bool match(std::initializer_list<TokenType> types);
    bool check(TokenType type) const;
    const Token& advance();
    bool isAtEnd() const;
    const Token& peek() const;
    const Token& previous() const;

    const Token& consume(TokenType type, const std::string& message);
    ParseError error(const Token& token, const std::string& message);
    void synchronize();

private:
    std::vector<Token> tokens;
    int current = 0;
};

} // namespace lox

#endif // PARSER_H
