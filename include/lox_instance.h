#ifndef LOX_INSTANCE_H
#define LOX_INSTANCE_H

#include <memory>
#include <string>

class LoxClass;

class LoxInstance {
public:
    explicit LoxInstance(std::shared_ptr<LoxClass> klass);
    std::string toString();

private:
    std::shared_ptr<LoxClass> klass;
};

#endif // LOX_INSTANCE_H
