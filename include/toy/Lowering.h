#ifndef TOY_LOWERING_H
#define TOY_LOWERING_H

#include "toy/AST.h"

#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"

namespace toy {

    class Lowering {
        public:
            explicit Lowering(mlir::MLIRContext &context);

            mlir::ModuleOp lower(const Program &program);

        private:
            mlir::MLIRContext &context;
            mlir::OpBuilder builder;
            void lowerFunction(mlir::ModuleOp module,const FunctionDecl &function);
    };

}

#endif