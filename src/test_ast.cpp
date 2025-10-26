#include "ast_printer.h"
#include "expr.h"
#include "token.h"
#include <iostream>
#include <memory>

int main() {
    using namespace lox;
    
    // Manually create expression: -123 * 45.67
    // This translates to: (* (- 123) (group 45.67))
    
    // Create the literals
    auto literal123 = std::make_unique<Literal>(123);
    auto literal4567 = std::make_unique<Literal>(45.67);
    
    // Create the unary expression: -123
    Token minus_token(TokenType::MINUS, "-", std::any(), 1);
    auto unary = std::make_unique<Unary>(minus_token, std::move(literal123));
    
    // Create the grouping expression: (45.67)
    auto grouping = std::make_unique<Grouping>(std::move(literal4567));
    
    // Create the binary expression: (-123) * (45.67)
    Token star_token(TokenType::STAR, "*", std::any(), 1);
    auto binary = std::make_unique<Binary>(std::move(unary), star_token, std::move(grouping));
    
    // Print the AST
    AstPrinter printer;
    std::cout << printer.print(*binary) << std::endl;
    
    return 0;
}
