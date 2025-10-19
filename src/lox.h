#ifndef LOX_H
#define LOX_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


namespace lox {

void runFile(const std::string& path);
void runPrompt();
void run(const std::string& source);

void error(int line, const std::string& message);
void report(int line, const std::string& where, const std::string& message);


}

#endif // LOX_H