#ifndef TOY_LOWERING_H
#define TOY_LOWERING_H

#include "toy/AST.h"
#include "toy/ToyOps.h"

#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/Arith/IR/Arith.h"

#include <unordered_map>
#include <string>

namespace toy {

    class Lowering {
        public:
            explicit Lowering(mlir::MLIRContext &context);

            mlir::ModuleOp lower(const Program &program);

        private:
            mlir::MLIRContext &context;
            mlir::OpBuilder builder;
            std::unordered_map<std::string, mlir::Value> symbolTable;
            void lowerFunction(mlir::ModuleOp module,const FunctionDecl &function);\
            mlir::Value lowerExpression(const Expression &expression);
            void lowerStatement(const Statement &statement);
            void lowerIfStatement(const IfStmt &stmt);
            void lowerWhileStatement(const WhileStmt &stmt);
    };

}

#endif