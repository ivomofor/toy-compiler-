#include "toy/Lexer.h"
#include "toy/Parser.h"
#include "toy/Lowering.h"

#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/BuiltinOps.h"

#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"

#include "mlir/Pass/PassManager.h"
#include "mlir/Transforms/Passes.h"

#include "mlir/Conversion/ArithToLLVM/ArithToLLVM.h"
#include "mlir/Conversion/FuncToLLVM/ConvertFuncToLLVM.h"
#include "mlir/Conversion/LLVMCommon/ConversionTarget.h"

#include "mlir/Support/LogicalResult.h"

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
    context.getOrLoadDialect<mlir::LLVM::LLVMDialect>();

    toy::Lowering lowering(context);

    mlir::ModuleOp module = lowering.lower(*program);

    if (mlir::failed(module.verify())) {
        std::cerr << "Error: Generated MLIR failed verification\n";
        return 1;
    }

    mlir::PassManager pm(&context);

    pm.addPass(mlir::createCanonicalizerPass());

    if (mlir::failed(pm.run(module))) {
        std::cerr << "Error: MLIR pass pipeline failed\n";
        return 1;
    }

    mlir::ConversionTarget target(context);

    target.addLegalDialect<mlir::LLVM::LLVMDialect>();
    target.addLegalOp<mlir::ModuleOp>();

    target.addIllegalDialect<mlir::func::FuncDialect>();
    target.addIllegalDialect<mlir::arith::ArithDialect>();

    mlir::RewritePatternSet patterns(&context);

    mlir::LLVMTypeConverter typeConverter(&context);

    mlir::arith::populateArithToLLVMConversionPatterns(typeConverter,patterns);

    mlir::populateFuncToLLVMConversionPatterns(typeConverter,patterns);

    if (mlir::failed(mlir::applyPartialConversion(module,target,std::move(patterns)))) {
        std::cerr << "Error: LLVM dialect conversion failed\n";
        return 1;
    }

    if (mlir::failed(module.verify())) {
        std::cerr << "Error: MLIR failed verification after passes\n";
        return 1;
    }

    module.dump();

    return 0;
}