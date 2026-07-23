cmake_minimum_required(VERSION 3.16)

project(ToyCompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------
# LLVM / MLIR
# ------------------------------------------------------------------

set(LLVM_DIR
    "/home/shevo/LLVM-22.1.0-Linux-X64/lib/cmake/llvm"
)

set(MLIR_DIR
    "/home/shevo/LLVM-22.1.0-Linux-X64/lib/cmake/mlir"
)

find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)

message(STATUS "Using LLVM ${LLVM_PACKAGE_VERSION}")

# ------------------------------------------------------------------
# LLVM / MLIR include directories
# ------------------------------------------------------------------

include_directories(
    ${LLVM_INCLUDE_DIRS}
    ${MLIR_INCLUDE_DIRS}
    /home/shevo/LLVM-22.1.0-Linux-X64/include
)

# ------------------------------------------------------------------
# ToyCompiler library
# ------------------------------------------------------------------

add_library(ToyCompiler
    lib/AST.cpp
    lib/ASTPrinter.cpp
    lib/Lexer.cpp
    lib/Parser.cpp
    lib/Lowering.cpp
)

target_include_directories(ToyCompiler
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${LLVM_INCLUDE_DIRS}
        ${MLIR_INCLUDE_DIRS}
        /home/shevo/LLVM-22.1.0-Linux-X64/include
)

# ------------------------------------------------------------------
# Link MLIR libraries
# ------------------------------------------------------------------

target_link_libraries(ToyCompiler
    PUBLIC
        MLIRIR
        MLIRParser
        MLIRSupport
        MLIRFuncDialect
        MLIRArithDialect
)

# ------------------------------------------------------------------
# Lexer executable
# ------------------------------------------------------------------

add_executable(toy-lex
    tools/toy-lex.cpp
)

target_link_libraries(toy-lex
    PRIVATE
        ToyCompiler
)

# ------------------------------------------------------------------
# Parser / MLIR executable
# ------------------------------------------------------------------

add_executable(toy-parse
    tools/toy-parse.cpp
)

target_link_libraries(toy-parse
    PRIVATE
        ToyCompiler
)