cmake_minimum_required(VERSION 3.10)
project(cli_tui C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)


# === Main TUI Framework ===
add_library(cli_tui
    src/cli_core.c
    src/screen.c
    src/input.c
 )

# === Hot Reloadable Plugin (.so) ===
add_library(hotreload_module SHARED
    src/hot_reload.c
)
set_target_properties(hotreload_module PROPERTIES
    OUTPUT_NAME "hotreload"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
  
# === Demo App ===
add_executable(demo demo/main.c)
add_library(hotreload_loader STATIC
    demo/hotreload_loader.c
)

target_link_libraries(demo PRIVATE cli_tui hotreload_loader dl)
add_dependencies(demo hotreload_module)

# === Compiler Flags ===
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(cli_tui PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(hotreload_module PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(demo PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
