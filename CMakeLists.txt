# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(termiui C CXX)

set(CMAKE_C_COMPILER "clang")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

add_executable(termiui_app
    src/main.c
    src/backstage.c
    src/director.c
    src/actors.c
    src/actor_casting.cpp
    src/conductor.c
)

target_include_directories(termiui_app PUBLIC "${CMAKE_SOURCE_DIR}/include")
