cmake_minimum_required(VERSION 3.10)
project(clox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main executable source files (excluding test files)
file(GLOB SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/test_ast.cpp")

# Create main executable
add_executable(clox ${SOURCES})

# Include directories
target_include_directories(clox PRIVATE src)
