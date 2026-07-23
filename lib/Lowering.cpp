#include "toy/Lowering.h"

#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Location.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"

namespace toy {

    Lowering::Lowering(mlir::MLIRContext &context)
        : context(context), builder(&context) {}

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
        auto i32Type = mlir::IntegerType::get(&context,32);
        auto funcType = mlir::FunctionType::get(&context, {}, i32Type );
        auto func = mlir::func::FuncOp::create(mlir::UnknownLoc::get(&context),function.name, funcType);
        auto *entryBlock = func.addEntryBlock();
        mlir::OpBuilder builder(&context);
        builder.setInsertionPointToEnd(entryBlock);
        for (const auto &statement :function.body) {
            lowerStatement(*statement);
        }
        module.push_back(func);
    }

    mlir::Value Lowering::lowerExpression(const Expression &expression) {
        auto *integer = dynamic_cast<const IntegerLiteral *>(&expression);
        if (integer) {
            auto type = mlir::IntegerType::get(&context,32);
            auto value = mlir::IntegerAttr::get(type,integer->value);
            return builder.create<mlir::arith::ConstantOp>(mlir::UnknownLoc::get(&context),type,value);
        }
        return {};
    }

    void Lowering::lowerStatement(const Statement &statement) {
        auto *returnStmt = dynamic_cast<const ReturnStmt *>(&statement);
        if (returnStmt) {
            mlir::Value value = lowerExpression(*returnStmt->value);
            builder.create<mlir::func::ReturnOp>(mlir::UnknownLoc::get(&context),value);
        }
    }
}