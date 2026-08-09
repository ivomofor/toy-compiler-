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

include(TableGen)
include(AddMLIR)
include(AddLLVM)

set(LLVM_TARGET_DEFINITIONS
        ${PROJECT_SOURCE_DIR}/dialects/ToyDialect.td
)

mlir_tablegen(ToyDialect.h.inc
        -gen-dialect-decls
        -I ${PROJECT_SOURCE_DIR}/dialects
)

mlir_tablegen(ToyDialect.cpp.inc
        -gen-dialect-defs
        -I ${PROJECT_SOURCE_DIR}/dialects
)

add_public_tablegen_target(ToyDialectIncGen)

set(TOY_OPS_TD
        ${PROJECT_SOURCE_DIR}/dialects/operations/ToyOps.td
)

set(LLVM_TARGET_DEFINITIONS ${TOY_OPS_TD})

mlir_tablegen(ToyOps.h.inc
        -gen-op-decls
        -I ${PROJECT_SOURCE_DIR}
)

mlir_tablegen(ToyOps.cpp.inc
        -gen-op-defs
        -I ${PROJECT_SOURCE_DIR}
)

add_public_tablegen_target(ToyOpsIncGen)

add_library(ToyCompiler
        lib/Lexer.cpp
        lib/Parser.cpp
        lib/AST.cpp
        lib/ASTPrinter.cpp
        lib/Lowering.cpp
        lib/ToyDialect.cpp
)

set_source_files_properties(
        lib/ToyDialect.cpp
        PROPERTIES
        COMPILE_OPTIONS "-fno-rtti"
)

add_dependencies(ToyCompiler ToyDialectIncGen ToyOpsIncGen)

target_include_directories(ToyCompiler
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
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
        MLIRIR
        MLIRTargetLLVMIRExport
        MLIRLLVMToLLVMIRTranslation
        MLIRToLLVMIRTranslationRegistration
)
