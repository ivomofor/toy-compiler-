cmake_minimum_required(VERSION 3.16)

project(ToyCompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH
    "${LLVM_CMAKE_DIR}"
    "${MLIR_CMAKE_DIR}"
)

include_directories(
    ${LLVM_INCLUDE_DIRS}
    ${MLIR_INCLUDE_DIRS}
)

add_definitions(${LLVM_DEFINITIONS})

add_library(ToyCompiler
    lib/Lexer.cpp
    lib/Parser.cpp
    lib/AST.cpp
    lib/ASTPrinter.cpp
    lib/Lowering.cpp
)

target_include_directories(ToyCompiler
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(ToyCompiler
    PUBLIC
        MLIRIR
        MLIRFuncDialect
        MLIRArithDialect
        MLIRPass
        MLIRTransforms
        MLIRArithToLLVM
        MLIRFuncToLLVM
        MLIRLLVMCommonConversion
        MLIRLLVMDialect
)

add_executable(toy-lex
    tools/toy-lex.cpp
)

target_link_libraries(toy-lex
    PRIVATE
        ToyCompiler
)

add_executable(toy-parse
    tools/toy-parse.cpp
)

target_link_libraries(toy-parse
    PRIVATE
        ToyCompiler
        MLIRTargetLLVMIRExport
        MLIRLLVMToLLVMIRTranslation
        MLIRToLLVMIRTranslationRegistration
)