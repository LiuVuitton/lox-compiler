#ifndef LOX_H
#define LOX_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "runtime_error.h"
#include "token.h"


namespace lox {

void runFile(const std::string& path);
void runPrompt();
void run(const std::string& source);

void error(int line, const std::string& message);
void error(const Token& token, const std::string& message);
void report(int line, const std::string& where, const std::string& message);


}

#endif // LOX_H