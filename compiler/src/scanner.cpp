#include "scanner.h"

Scanner::Scanner(const std::string& src)
    : start(0), current(0), line(1) {
    this->source = src;
}

bool Scanner::isAlpha(char c) const {
    return (
        ('a' <= c && c <= 'z') ||
        ('A' <= c && c <= 'Z') ||
        c == '_'
    );
}

bool Scanner::isDigit(char c) const {
    return '0' <= c && c <= '9';
}

bool Scanner::isAtEnd() const {
    return current >= source.size();
}

Token Scanner::makeToken(TokenType type) const {
    return { type, source.substr(start, current - start), line };
}

Token Scanner::errorToken(const std::string& msg) const {
    return { TokenType::ERROR, msg, line };
}

void Scanner::skipWhitespace() {
    while (true) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                ++line;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                }
                else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

TokenType Scanner::checkKeyword(const std::string& lexeme, std::size_t start,
                           const std::string& rest, TokenType type) const {
    if (lexeme.size() == start + rest.size() &&
        lexeme.substr(start, rest.size()) == rest) {
        return type;
    }
    return TokenType::IDENTIFIER;
}


TokenType Scanner::identifierType() const {
    std::string lexeme = source.substr(start, current - start);

    switch (lexeme[0]) {
        case 'a': return checkKeyword(lexeme, 1, "nd", TokenType::AND);
        case 'c': return checkKeyword(lexeme, 1, "lass", TokenType::CLASS);
        case 'e': return checkKeyword(lexeme, 1, "lse", TokenType::ELSE);
        case 'f': 
            if (lexeme.size() > 1) {
                switch (lexeme[1]) {
                    case 'a': return checkKeyword(lexeme, 2, "lse", TokenType::FALSE);
                    case 'o': return checkKeyword(lexeme, 2, "r", TokenType::FOR);
                    case 'u': return checkKeyword(lexeme, 2, "n", TokenType::FUN);
                }
            }
            break;
        case 'i': return checkKeyword(lexeme, 1, "f", TokenType::IF);
        case 'n': return checkKeyword(lexeme, 1, "il", TokenType::NIL);
        case 'o': return checkKeyword(lexeme, 1, "r", TokenType::OR);
        case 'p': return checkKeyword(lexeme, 1, "rint", TokenType::PRINT);
        case 'r': return checkKeyword(lexeme, 1, "eturn", TokenType::RETURN);
        case 's': return checkKeyword(lexeme, 1, "uper", TokenType::SUPER);
        case 't': 
            if (lexeme.size() > 1) {
                switch (lexeme[1]) {
                    case 'h': return checkKeyword(lexeme, 2, "is", TokenType::THIS);
                    case 'r': return checkKeyword(lexeme, 2, "ue", TokenType::TRUE);
                }
            }
            break;
        case 'v': return checkKeyword(lexeme, 1, "ar", TokenType::VAR);
        case 'w': return checkKeyword(lexeme, 1, "hile", TokenType::WHILE);
    }
    return TokenType::IDENTIFIER;
}


Token Scanner::identifier() {
    while (isAlpha(peek()) || isDigit(peek())) advance();

    return makeToken(identifierType());
}

Token Scanner::number() {
    while (isDigit(peek())) advance();

    // Check for fractional part
    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        
        while (isDigit(peek())) advance();
    }

    return makeToken(TokenType::NUMBER);
}

Token Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') ++line;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string.");

    advance();
    return makeToken(TokenType::STRING);
}

Token Scanner::scanToken() {
    if (isAtEnd()) return makeToken(TokenType::END_OF_FILE);
    skipWhitespace();
    start = current;

    char c = advance();

    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    switch (c) {
        case '(': return makeToken(TokenType::LEFT_PAREN);
        case ')': return makeToken(TokenType::RIGHT_PAREN);
        case '{': return makeToken(TokenType::LEFT_BRACE);
        case '}': return makeToken(TokenType::RIGHT_BRACE);
        case ';': return makeToken(TokenType::SEMICOLON);
        case ',': return makeToken(TokenType::COMMA);
        case '.': return makeToken(TokenType::DOT);
        case '-': return makeToken(TokenType::MINUS);
        case '+': return makeToken(TokenType::PLUS);
        case '/': return makeToken(TokenType::SLASH);
        case '*': return makeToken(TokenType::STAR);
        case '!':
            return makeToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        case '=':
            return makeToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        case '<':
            return makeToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        case '>':
            return makeToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        case '"': return string();
    }

    return errorToken("Unexpected character.");
}

char Scanner::advance() {
    return source[current++];
}

char Scanner::peek() const {
    return source[current];
}

char Scanner::peekNext() const {
    if (isAtEnd()) return '\0';
    return source[current + 1];
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    ++current;
    return true;
}