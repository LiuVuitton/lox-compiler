#ifndef LOX_H
#define LOX_h

#include <string>

bool had_error = false;

void runFile(const std::string& path);
void runPrompt();
void run(const std::string& source);
void error(int line, const std::string& message);
void report(int line, const std::string& where, const std::string& message);

#endif // LOX_H