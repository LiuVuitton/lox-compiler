#include "token_type.h"

std::string to_string(TokenType type) {
    switch (type) {
#define X(name) case TokenType::name: return #name;
        TOKEN_TYPE_LIST
#undef X
        default: return "UNKNOWN";
    }
}
