#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "token_type.h"
#include "expr.h"
#include "stmt.h"
#include <memory>
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <string>

struct ParseError {};

class Parser {
public:
    Parser(std::vector<Token>& tokens);
    // std::unique_ptr<Expr> parse();
    std::vector<std::unique_ptr<Stmt>> parse();

private:
    std::vector<Token> tokens;
    int current{0};

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> expressionStatement();
    std::vector<std::unique_ptr<Stmt>> block();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
    bool match(std::initializer_list<TokenType> types);
    Token consume(TokenType type, const std::string& message);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    ParseError error(Token token, const std::string& message);
    void synchronize();
};

#endif // PARSER_H