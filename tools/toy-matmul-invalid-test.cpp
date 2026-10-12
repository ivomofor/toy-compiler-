//
// Created by shevo on 10/11/26.
//

#include "toy/ToyDialect.h"
#include "toy/TensorMatmulToLinalg.h"

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/Verifier.h"
#include "mlir/Parser/Parser.h"
#include "mlir/Pass/PassManager.h"

#include "llvm/Support/raw_ostream.h"

int main() {
    mlir::MLIRContext context;

    context.loadDialect<toy::ToyDialect>();
    context.loadDialect<mlir::func::FuncDialect>();
    context.loadDialect<mlir::arith::ArithDialect>();
    context.loadDialect<mlir::tensor::TensorDialect>();
    context.loadDialect<mlir::linalg::LinalgDialect>();

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
        llvm::errs() << "Error: Input module failed verification\n";
        return 1;
    }

    mlir::PassManager pm(&context);
    pm.addPass(toy::createTensorMatmulToLinalgPass());

    if (mlir::failed(pm.run(*module))) {
        llvm::errs() << "Error: Matmul lowering failed\n";
        return 1;
    }

    int toyMatmulCount = 0;
    int linalgMatmulCount = 0;

    bool found2x2 = false;
    bool found2x4 = false;
    bool found4x3 = false;

    module->walk([&](mlir::Operation *op) {
        auto name = op->getName().getStringRef();

        if (name == "toy.tensor.matmul") {
            ++toyMatmulCount;
        }

        if (name == "linalg.matmul") {
            ++linalgMatmulCount;

            auto resultType = mlir::cast<mlir::RankedTensorType>(
                op->getResult(0).getType());

            if (resultType.getShape() ==
                llvm::ArrayRef<int64_t>({2, 2})) {
                found2x2 = true;
            }

            if (resultType.getShape() ==
                llvm::ArrayRef<int64_t>({2, 4})) {
                found2x4 = true;
            }

            if (resultType.getShape() ==
                llvm::ArrayRef<int64_t>({4, 3})) {
                found4x3 = true;
            }
        }
    });

    if (toyMatmulCount != 0) {
        llvm::errs() << "Error: Toy matmul operations remain\n";
        return 1;
    }

    if (linalgMatmulCount != 3) {
        llvm::errs() << "Error: Expected three linalg.matmul operations\n";
        return 1;
    }

    if (!found2x2 || !found2x4 || !found4x3) {
        llvm::errs() << "Error: Expected result shapes were not all found\n";
        return 1;
    }

    if (mlir::failed(mlir::verify(*module))) {
        llvm::errs() << "Error: Lowered module failed verification\n";
        return 1;
    }

    llvm::outs() << "All valid matmul shapes lowered successfully\n";
    module->print(llvm::outs());
    llvm::outs() << "\n";

    return 0;
}