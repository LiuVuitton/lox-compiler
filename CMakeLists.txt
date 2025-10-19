cmake_minimum_required(VERSION 3.10)
project(clox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add all source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(clox ${SOURCES})

# Include directories
target_include_directories(clox PRIVATE src)
