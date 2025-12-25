#ifndef LOX_H
#define LOX_H

#include <string>
#include "token.h"
#include "interpreter.h"
#include "runtime_error.h"

namespace Lox {

extern Interpreter interpreter;
extern bool had_error;
extern bool had_runtime_error;

void runFile(const std::string& path);
void runPrompt();
void run(const std::string& source);
void error(int line, const std::string& message);
void report(int line, const std::string& where, const std::string& message);
void error(Token token, const std::string& message);
void runtimeError(const RuntimeError& error);

} // namespace Lox

#endif // LOX_H