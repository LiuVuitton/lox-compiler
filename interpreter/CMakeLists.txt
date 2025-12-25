cmake_minimum_required(VERSION 3.10)

project(interpreter
    VERSION 0.1
    LANGUAGES CXX
)

# Collect all source files
file(GLOB_RECURSE INTERPRETER_SOURCES
    "src/*.cpp"
)

# Create the interpreter executable
add_executable(lox
    main.cpp
    ${INTERPRETER_SOURCES}
)

# Set include directories
target_include_directories(lox PRIVATE include)

# Set C++ standard if needed
target_compile_features(lox PRIVATE cxx_std_17)

