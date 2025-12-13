#ifndef UTILITY_H
#define UTILITY_H

#include "token_type.h"
#include <string>
#include <any>

std::string tokenTypeToString(const TokenType& type);
std::string literalToString(const std::any& literal);

#endif // UTILITY_H