#include <cctype>
#include <string>
#include <cstdlib>

#include "scanner.h"
#include "token.h"
#include "lox.h"


namespace lox {

Scanner::Scanner(const std::string& source)
    : source(source) {}


std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        Scanner::scanToken();
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, "", {}, line));
    return tokens;
}

bool Scanner::isAtEnd() const {
    return current >= source.size();
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
    case '(': addToken(TokenType::LEFT_PAREN); break;
    case ')': addToken(TokenType::RIGHT_PAREN); break;
    case '{': addToken(TokenType::LEFT_BRACE); break;
    case '}': addToken(TokenType::RIGHT_BRACE); break;
    case ',': addToken(TokenType::COMMA); break;
    case '.': addToken(TokenType::DOT); break;
    case '-': addToken(TokenType::MINUS); break;
    case '+': addToken(TokenType::PLUS); break;
    case ';': addToken(TokenType::SEMICOLON); break;
    case '*': addToken(TokenType::STAR); break;
    case '!':
        addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
    case '=':
        addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
    case '<':
        addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
    case '>':
        addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;
    case '/':
        if (match('/')) {
            // A comment goes until the end of the line
            while (peek() != '\n' && !isAtEnd()) advance();
        }
        else {
            addToken(TokenType::SLASH);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace
        break;
    case '\n':
        ++line;
        break;
    case '"': string(); break;

    default:
        if (isDigit(c)) {
            number();
        }
        else if (isAlpha(c)) {
            identifier();
        }
        else {
            error(line, "Unexpected character.");
            break;
        }
    }
}

char Scanner::advance() {
    ++current;
    return source[current-1];
}

void Scanner::addToken(TokenType type) {
    addToken(type, {}); // empty std::any acts like null
}

void Scanner::addToken(TokenType type, std::any literal) {
    std::string text = source.substr(start, current - start);
    tokens.push_back(Token(type, text, literal, line));
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    ++current;
    return true;
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') ++line;
        advance();
    }

    if (isAtEnd()) {
        error(line, "Unterminated string.");
        return;
    }
    advance();
    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

void Scanner::number() {
    while (isDigit(peek())) advance();

    // Look for a fractional part
    if (peek() == '.' && isDigit(peekNext())) {
        advance();  // Consume the '.'

        while (isDigit(peek())) advance();
    }

    // Extract the substring and convert to double
    double value = std::stod(source.substr(start, current - start));

    addToken(TokenType::NUMBER, value);
}

char Scanner::peekNext() {
    if (current + 1 >= source.size()) return '\0';
    return source[current + 1];
}

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) advance();

    std::string text = source.substr(start, current - start);

    TokenType type;
    auto it = keywords.find(text);
    if (it != keywords.end()) {
        type = it->second; // found in keywords map
    } else {
        type = TokenType::IDENTIFIER; // default
    }

    addToken(type);
}

bool Scanner::isAlpha(char c) {
    return (
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_'
    );
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

}