cmake_minimum_required(VERSION 3.16)

project(ToyCompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(toy-lex
    tools/toy-lex.cpp
    lib/Lexer.cpp
)

target_include_directories(toy-lex PRIVATE
    include
)