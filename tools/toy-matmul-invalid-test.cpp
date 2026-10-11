//
// Created by shevo on 10/11/26.
//

#include "toy/ToyDialect.h"

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/Verifier.h"
#include "mlir/Parser/Parser.h"

#include "llvm/Support/raw_ostream.h"

int main() {
    mlir::MLIRContext context;

    context.loadDialect<toy::ToyDialect>();
    context.loadDialect<mlir::func::FuncDialect>();
    context.loadDialect<mlir::arith::ArithDialect>();
    context.loadDialect<mlir::tensor::TensorDialect>();

    mlir::ParserConfig config(&context);

    auto module =
        mlir::parseSourceFile<mlir::ModuleOp>(
            "example/tensor_matmul_invalid.mlir",
            config);

    if (!module) {
        llvm::outs() << "Invalid matmul correctly rejected\n";
        return 1;
    }

    if (mlir::succeeded(mlir::verify(*module))) {
        llvm::errs() << "Error: Invalid matmul unexpectedly passed verification\n";
        return 1;
    }

    llvm::outs() << "Invalid matmul correctly rejected\n";

    return 0;
}
