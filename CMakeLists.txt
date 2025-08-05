# Specifies the minimum version of CMake required to build the project.
cmake_minimum_required(VERSION 3.10)
# Defines the project name and the languages used.
project(termiui C CXX)

# Sets the C compiler to Clang.
set(CMAKE_C_COMPILER "clang")

# Defines the output directories for the executable and library files.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# Create the executable directly from all source files.
# This simpler approach avoids potential linking issues with a static library
# in a mixed-language project by compiling and linking everything in a single step.
add_executable(termiui_app
    src/main.c
    src/backstage.c
    src/director.c
    src/actors.c
    src/actor_casting.cpp
    src/conductor.c
    src/utils.c
)

# Specify the include directories for the executable.
target_include_directories(termiui_app PUBLIC "${CMAKE_SOURCE_DIR}/include")
