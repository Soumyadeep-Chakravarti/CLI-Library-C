cmake_minimum_required(VERSION 3.10)
project(cli_tui C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_library(cli_tui
    src/cli_core.c
    src/screen.c
    src/input.c
)

add_executable(demo demo/main.c)

target_link_libraries(demo PRIVATE cli_tui)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(cli_tui PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(demo PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
