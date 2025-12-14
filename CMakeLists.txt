cmake_minimum_required(VERSION 3.10)

project(lox
    VERSION 0.1
    LANGUAGES CXX
)

add_executable(lox
    src/expr.cpp
    src/lox.cpp
    src/main.cpp
    src/parser.cpp
    src/scanner.cpp
    src/token_type.cpp
    src/token.cpp
    src/utility.cpp
    src/ast_printer.cpp
    src/runtime_error.cpp
    src/interpreter.cpp
    src/stmt.cpp
    src/environment.cpp
    
)

target_include_directories(lox PRIVATE include)
