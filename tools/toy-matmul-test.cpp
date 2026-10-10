#include "toy/ToyDialect.h"
#include "toy/TensorMatmulToLinalg.h"

#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
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
            "example/tensor_matmul.mlir",
            config);

    if (!module) {
        llvm::errs() << "Error: Could not parse MLIR file\n";
        return 1;
    }

    mlir::PassManager pm(&context);
    pm.addPass(toy::createTensorMatmulToLinalgPass());

    if (mlir::failed(pm.run(*module))) {
        llvm::errs() << "Error: Matmul lowering failed\n";
        return 1;
    }

    bool foundToyMatmul = false;
    bool foundLinalgMatmul = false;

    module->walk([&](mlir::Operation *op) {
        auto name = op->getName().getStringRef();

        if (name == "toy.tensor.matmul") {
            foundToyMatmul = true;
        }

        if (name == "linalg.matmul") {
            foundLinalgMatmul = true;
        }
    });

    if (foundToyMatmul) {
        llvm::errs() << "Error: toy.tensor.matmul was not lowered\n";
        return 1;
    }

    if (!foundLinalgMatmul) {
        llvm::errs() << "Error: linalg.matmul was not generated\n";
        return 1;
    }

    if (mlir::failed(mlir::verify(*module))) {
        llvm::errs() << "Error: Transformed module failed verification\n";
        return 1;
    }

    llvm::outs() << "Matmul lowering test passed\n";
    module->print(llvm::outs());
    llvm::outs() << "\n";

    return 0;
}