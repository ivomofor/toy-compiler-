#include "toy/Lexer.h"
#include "toy/Parser.h"
#include "toy/Lowering.h"

#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cerr << "Usage: toy-parse <file.toy>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: Could not open file: " << filename << "\n";
        return 1;
    }

    std::string source((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());

    toy::Lexer lexer(source);

    toy::Parser parser(lexer);

    auto program = parser.parseProgram();

    if (!program) {
        std::cerr << "Error: Failed to parse program\n";
        return 1;
    }

    mlir::MLIRContext context;

    context.getOrLoadDialect<mlir::func::FuncDialect>();

    context.getOrLoadDialect<mlir::arith::ArithDialect>();

    toy::Lowering lowering(context);

    mlir::ModuleOp module = lowering.lower(*program);

    module.dump();

    return 0;
}