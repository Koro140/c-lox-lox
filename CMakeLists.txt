cmake_minimum_required(VERSION 3.10)

project(Lox)

set(
    SRC
    src/main.cpp
    src/Scanner.cpp
)

include_directories(src)

add_executable(Lox ${SRC})
add_compile_definitions(Lox SCRIPT_PATH="${CMAKE_SOURCE_DIR}/")