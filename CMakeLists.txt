cmake_minimum_required(VERSION 3.10)

project(Lox)
project(AST_Gen)

set(
    SRC_LOX
    src/main.cpp
    src/Scanner.cpp
    src/Parser.cpp
)

set(
    SRC_AST_GEN
    ast-gen/main.cpp
)

include_directories(src)

add_executable(Lox ${SRC_LOX})
add_compile_definitions(Lox SCRIPT_PATH="${CMAKE_SOURCE_DIR}/")

add_executable(AST_Gen ${SRC_AST_GEN})