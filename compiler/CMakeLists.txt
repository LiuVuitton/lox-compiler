cmake_minimum_required(VERSION 3.10)

project(compiler
    VERSION 0.1
    LANGUAGES CXX
)

# Compiler CMakeLists.txt
# Placeholder for future compiler implementation

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

# Set C++ standard if needed
target_compile_features(lox PRIVATE cxx_std_17)

