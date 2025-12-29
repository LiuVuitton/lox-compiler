set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.10)

project(compiler
    VERSION 0.1
    LANGUAGES CXX
)

# Collect all source files
file(GLOB_RECURSE COMPILER_SOURCES
    "src/*.cpp"
)

# Create the compiler executable
add_executable(lox
    main.cpp
    ${COMPILER_SOURCES}
)

# Set include directories
target_include_directories(lox PRIVATE include)

# Set C++20 standard
set_target_properties(lox PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
