#include "toy/Lowering.h"

#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Location.h"

namespace toy {

    Lowering::Lowering(mlir::MLIRContext &context)
        : context(context) {}

    mlir::ModuleOp Lowering::lower(const Program &program) {
        (void)program;
        return mlir::ModuleOp::create(mlir::UnknownLoc::get(&context)
        );
    }
}