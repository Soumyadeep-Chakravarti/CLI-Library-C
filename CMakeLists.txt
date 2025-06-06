# ~/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(tui_framework C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Recursively grab all source files under src/
file(GLOB_RECURSE TUI_SOURCES "src/*.c")

# Create the core library from all src files
add_library(tui_core ${TUI_SOURCES})

# Make sure the compiler can find headers under include/
target_include_directories(tui_core PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Example executable from examples/hello_tui
add_executable(hello_tui examples/hello_tui/main.c)

# Link your core library to the executable
target_link_libraries(hello_tui PRIVATE tui_core)

# Include headers for the example executable as well
target_include_directories(hello_tui PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Compiler warnings and error flags for GCC/Clang
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(tui_core PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(hello_tui PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
