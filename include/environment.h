#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <unordered_set>
#include <any>
#include <string>
#include "token.h"

class Environment {
public:
    std::any get(const Token& name);
    void define(const std::string& name, std::any value);
    void assign(const Token& name, std::any value);

private:
    std::unordered_map<std::string, std::any> values;

};


#endif // ENVIRONMENT_H