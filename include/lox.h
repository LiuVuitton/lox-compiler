#ifndef LOX_H
#define LOX_h

#include <string>

bool had_error = false;

void runFile(std::string path);
void runPrompt();
void run(std::string source);
void error(int line, std::string message);
void report(int line, std::string where, std::string message);

#endif // LOX_H