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

    const char *files[] = {
        "example/tensor_matmul_invalid.mlir",
        "example/tensor_matmul_invalid_rank.mlir",
        "example/tensor_matmul_invalid_types.mlir",
        "example/tensor_matmul_invalid_result.mlir"
    };

    for (const char *file : files) {
        auto module =
            mlir::parseSourceFile<mlir::ModuleOp>(
                file,
                config);

        if (module) {
            llvm::errs()
                << "Error: Invalid matmul unexpectedly passed: "
                << file << "\n";
            return 1;
        }

        llvm::outs() << "Invalid matmul correctly rejected: "
                     << file << "\n";
    }

    return 0;
}