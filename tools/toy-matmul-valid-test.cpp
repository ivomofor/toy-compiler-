//
// Created by shevo on 10/12/26.
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
            "example/tensor_matmul_valid_shapes.mlir",
            config);

    if (!module) {
        llvm::errs() << "Error: Could not parse valid matmul examples\n";
        return 1;
    }

    if (mlir::failed(mlir::verify(*module))) {
        llvm::errs() << "Error: Valid matmul examples failed verification\n";
        return 1;
    }

    llvm::outs() << "Valid matmul shapes passed verification\n";
    module->print(llvm::outs());
    llvm::outs() << "\n";

    return 0;
}