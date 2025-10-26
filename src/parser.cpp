#include "parser.h"
#include "lox.h"  // already provides Lox::error(token, message)

namespace lox {

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens) {}

std::unique_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (const ParseError&) {
        return nullptr;
    }
}

// ─────────────── Grammar Methods ───────────────

std::unique_ptr<Expr> Parser::expression() { return equality(); }

std::unique_ptr<Expr> Parser::equality() {
    auto expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        auto right = comparison();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    auto expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL,
                  TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        auto right = term();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    auto expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        auto right = factor();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    auto expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        auto right = unary();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        auto right = unary();
        return std::make_unique<Unary>(op, std::move(right));
    }

    return primary();
}

std::unique_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_unique<Literal>(false);
    if (match({TokenType::TRUE}))  return std::make_unique<Literal>(true);
    if (match({TokenType::NIL}))   return std::make_unique<Literal>(nullptr);

    if (match({TokenType::NUMBER, TokenType::STRING}))
        return std::make_unique<Literal>(previous().literal);

    if (match({TokenType::LEFT_PAREN})) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");
}

// ─────────────── Utility Methods ───────────────

bool Parser::match(std::initializer_list<TokenType> types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

const Token& Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

const Token& Parser::peek() const {
    return tokens[current];
}

const Token& Parser::previous() const {
    return tokens[current - 1];
}

const Token& Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

Parser::ParseError Parser::error(const Token& token, const std::string& message) {
    error(token, message);  // uses your existing global error reporter
    return ParseError(message);
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;

        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            default:
                break;
        }

        advance();
    }
}

} // namespace lox
