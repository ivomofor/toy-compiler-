
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
            "test",
            {
                builder.getStringAttr("a"),
                builder.getStringAttr("b")
            });

    module.push_back(function);

    if (mlir::succeeded(function.verify())) {
        std::cout << "Verification succeeded as expected\n";
        return 0;
    }

    std::cout << "Verification failed unexpectedly\n";
    return 1;
}
