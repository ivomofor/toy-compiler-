
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

    auto &entryBlock = function.getBody().front();

    builder.setInsertionPointToEnd(&entryBlock);

    auto add = toy::AddOp::create(
        builder,
        builder.getUnknownLoc(),
        builder.getI32Type(),
        entryBlock.getArgument(0),
        entryBlock.getArgument(1));


    auto interface =
    mlir::dyn_cast<toy::ExampleInterface>(
        add.getOperation());

    if (interface) {
        std::cout << "Interface name: "
                  << interface.getToyName().str()
                  << "\n";
    }

    if (mlir::succeeded(function.verify())) {
        std::cout << "Verification succeeded as expected\n";
        return 0;
    }

    std::cout << "Verification failed unexpectedly\n";
    return 1;
}
