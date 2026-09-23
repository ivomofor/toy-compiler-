#include "toy/ToyDialect.h"
#include "toy/ToyOps.h"

#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Builders.h"

#include <iostream>

int main() {
    mlir::MLIRContext context;

    context.loadDialect<toy::ToyDialect>();

    mlir::OpBuilder builder(&context);

    auto module =
        mlir::ModuleOp::create(
            builder.getUnknownLoc());

    auto function =
        toy::FuncOp::create(
            builder,
            builder.getUnknownLoc(),
            builder.getStringAttr("broken"),
            builder.getArrayAttr({}));

    module.push_back(function);

    if (mlir::failed(function.verify())) {
        std::cout << "Verification failed as expected\n";
        return 0;
    }

    std::cout << "Verification unexpectedly succeeded\n";
    return 1;
}