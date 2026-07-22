#include "toy/Lowering.h"

#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Location.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"

namespace toy {

    Lowering::Lowering(mlir::MLIRContext &context)
        : context(context) {}

    mlir::ModuleOp Lowering::lower(const Program &program) {

        auto module = mlir::ModuleOp::create(mlir::UnknownLoc::get(&context));

        for (const auto &decl : program.declarations) {
            auto *function = dynamic_cast<FunctionDecl *>(decl.get());

        if (function) {
                lowerFunction(module,*function);
            }
        }

        return module;
    }

    void Lowering::lowerFunction(mlir::ModuleOp module, const FunctionDecl &function) {

        auto funcType = mlir::FunctionType::get(&context, {}, {} );

        auto func = mlir::func::FuncOp::create(mlir::UnknownLoc::get(&context),function.name, funcType);

        auto *entryBlock = func.addEntryBlock();
        
        module.push_back(func);
    }
}