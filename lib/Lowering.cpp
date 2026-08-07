#include "toy/Lowering.h"

#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Location.h"

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/SCF/IR/SCF.h"

#include <iostream>
#include <stdexcept>

namespace toy {

    Lowering::Lowering(mlir::MLIRContext &context)
        : context(context),builder(&context) {}

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

    void Lowering::lowerFunction(mlir::ModuleOp module,const FunctionDecl &function) {

        symbolTable.clear();

        auto i32Type = mlir::IntegerType::get( &context, 32 );
        auto funcType = mlir::FunctionType::get(&context,{},i32Type);
        auto func = mlir::func::FuncOp::create(mlir::UnknownLoc::get(&context),function.name,funcType);
        auto *entryBlock = func.addEntryBlock();

        builder.setInsertionPointToEnd(entryBlock);

        for (const auto &statement :function.body)
            lowerStatement(*statement);

        module.push_back(func);

        if (function.body.empty())
            throw std::runtime_error("Function contains no body");
    }

    mlir::Value Lowering::lowerExpression(const Expression &expression) {
        //std::cerr << "DEBUG: Entered lowerExpression()\n";
        auto *integer = dynamic_cast<const IntegerLiteral *>(&expression);

        if (integer) {
            //std::cerr << "DEBUG: IntegerLiteral detected: " << integer->value<< "\n";
            auto type = mlir::IntegerType::get(&context,32);
            auto value = mlir::IntegerAttr::get(type,integer->value);

            return mlir::arith::ConstantOp::create(builder,mlir::UnknownLoc::get(&context),type,value);
        }

        auto *binary = dynamic_cast<const BinaryExpression *>(&expression);

        if (binary) {
            //std::cerr << "DEBUG: BinaryExpression detected: " << binary->op<< "\n";
            mlir::Value left = lowerExpression(*binary->left);
            mlir::Value right = lowerExpression(*binary->right);

            switch (binary->op) {
                case '+':
                    return mlir::arith::AddIOp::create(builder,mlir::UnknownLoc::get(&context),left,right);
                case '-':
                    return mlir::arith::SubIOp::create(builder,mlir::UnknownLoc::get(&context),left,right);
                case '*':
                    return mlir::arith::MulIOp::create(builder,mlir::UnknownLoc::get(&context),left,right);
                case '/':
                    return mlir::arith::DivSIOp::create(builder,mlir::UnknownLoc::get(&context),left,right);
                default:
                    throw std::runtime_error("Unsupported binary operator");
            }
        }

        auto call = dynamic_cast<const CallExpression *>(&expression);

        if (call) {
            std::vector<mlir::Value> arguments;
            for (const auto &argument : call->arguments) {
                arguments.push_back(lowerExpression(*argument));
            }
            auto callee = builder.getStringAttr(call->callee);
            auto callOp = mlir::func::CallOp::create(builder,builder.getUnknownLoc(),call->callee,{builder.getI32Type()},arguments);
            return callOp.getResult(0);
        }

        auto *variable = dynamic_cast<const VariableReference *>(&expression);

        if (variable) {
            auto it = symbolTable.find(variable->name);
            if (it == symbolTable.end()) {
                throw std::runtime_error("Undefined variable: " +variable->name);
            }
            return it->second;
        }
        throw std::runtime_error("Unsupported expression in lowering");
    }

    void Lowering::lowerStatement(const Statement &statement) {

        auto *variableDecl = dynamic_cast<const VariableDecl *>(&statement);

        if (variableDecl) {
            mlir::Value value = lowerExpression(*variableDecl->initializer);
            symbolTable[variableDecl->name] = value;
            return;
        }

        auto ifStmt = dynamic_cast<const IfStmt *>(&statement);

        if (ifStmt) {
            lowerIfStatement(*ifStmt);
            return;
        }

        auto whileStmt = dynamic_cast<const WhileStmt *>(&statement);

        if (whileStmt) {
            lowerWhileStatement(*whileStmt);
            return;
        }

        auto *returnStmt = dynamic_cast<const ReturnStmt *>(&statement);

        if (returnStmt) {

            mlir::Value value =lowerExpression(*returnStmt->value);

            if (!value)
                throw std::runtime_error("Failed to lower return expression");

            mlir::func::ReturnOp::create(builder,mlir::UnknownLoc::get(&context),value);

            return;
        }
        throw std::runtime_error("Unsupported statement in lowering");
    }

    void Lowering::lowerIfStatement(const IfStmt &stmt) {

        mlir::Value condition = lowerExpression(*stmt.condition);
        auto ifOp = mlir::scf::IfOp::create(builder,builder.getUnknownLoc(), condition,false);
        builder.setInsertionPointToStart(&ifOp.getThenRegion().front());

        for (const auto &statement : stmt.thenBody) {
            lowerStatement(*statement);
        }
        builder.setInsertionPointAfter(ifOp);
    }

    void Lowering::lowerWhileStatement(const WhileStmt &stmt) {

        auto whileOp = mlir::scf::WhileOp::create(builder,builder.getUnknownLoc(),mlir::TypeRange{},mlir::ValueRange{});

        auto &beforeRegion = whileOp.getBefore();
        auto &afterRegion  = whileOp.getAfter();

        builder.createBlock(&beforeRegion);
        builder.setInsertionPointToStart(&beforeRegion.front());

        auto condition = lowerExpression(*stmt.condition);

        mlir::scf::ConditionOp::create(builder,builder.getUnknownLoc(),condition,mlir::ValueRange{});
        builder.createBlock(&afterRegion);
        builder.setInsertionPointToStart(&afterRegion.front());

        for (const auto &statement : stmt.body)
            lowerStatement(*statement);

        mlir::scf::YieldOp::create(builder,builder.getUnknownLoc());
        builder.setInsertionPointAfter(whileOp);
    }

}