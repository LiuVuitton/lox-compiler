#ifndef LOX_H
#define LOX_H

#include <string>

namespace Lox {
    
bool had_error = false;

void runFile(const std::string& path);
void runPrompt();
void run(const std::string& source);
void error(int line, const std::string& message);
void report(int line, const std::string& where, const std::string& message);
void error(Token token, const std::string& message);

} // namespace Lox

#endif // LOX_H