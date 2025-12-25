#ifndef LOX_INSTANCE_H
#define LOX_INSTANCE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <any>
#include "token.h"

class LoxClass;
class LoxFunction;

class LoxInstance : public std::enable_shared_from_this<LoxInstance> {
public:
    explicit LoxInstance(std::shared_ptr<LoxClass> klass);

    std::any get(const Token& name);
    void set(const Token& name, std::any value);
    std::string toString();

private:
    std::shared_ptr<LoxClass> klass;
    std::unordered_map<std::string, std::any> fields;
};

#endif // LOX_INSTANCE_H
