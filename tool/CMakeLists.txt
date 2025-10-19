cmake_minimum_required(VERSION 3.10)
project(generate_ast)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create the AST generator tool
add_executable(generate_ast generate_ast.cpp)
target_include_directories(generate_ast PRIVATE src)
