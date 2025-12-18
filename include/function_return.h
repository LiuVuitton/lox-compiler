#ifndef RETURN_EXCEPTION_H
#define RETURN_EXCEPTION_H

#include <any>

class FunctionReturn {
public:
    std::any value;
    explicit FunctionReturn(std::any value);
};


#endif // RETURN_EXCEPTION_H