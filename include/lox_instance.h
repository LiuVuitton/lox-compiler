#ifndef LOX_INSTANCE_H
#define LOX_INSTANCE_H

#include <memory>
#include <string>
#include "token.h"

class LoxClass;

class LoxInstance {
public:
    explicit LoxInstance(std::shared_ptr<LoxClass> klass);
    std::any get(const Token& name);
    std::string toString();

private:
    std::shared_ptr<LoxClass> klass;
    std::unordered_map<std::string, std::any> fields;
};

#endif // LOX_INSTANCE_H
