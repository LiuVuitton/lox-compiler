#include "lox_instance.h"
#include "lox_class.h"

LoxInstance::LoxInstance(std::shared_ptr<LoxClass> klass)
    : klass(std::move(klass)) {}

std::string LoxInstance::toString() {
    return klass->toString() + " instance";
}