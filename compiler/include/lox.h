#pragma once

#include <string>
#include "vm.h"

namespace Lox {

void repl(VM& vm);
int runFile(VM& vm, const std::string& path);
std::string readFile(const std::string& path);

}