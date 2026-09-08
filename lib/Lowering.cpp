#include "toy/Lowering.h"
#include "toy/ToyOps.h"

#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Location.h"

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/SCF/IR/SCF.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"

#include <iostream>
#include <stdexcept>

namespace toy {

    Lowering::Lowering(mlir::MLIRContext &context)
        : context(context),builder(&context) {}

        mlir::ModuleOp Lowering::lower(const Program &program) {

            auto module = mlir::ModuleOp::create(mlir::UnknownLoc::get(&context));

        for (const auto &decl : program.declarations) {
            if (decl->getKind() == ASTNodeKind::FunctionDecl) {
                auto *function = static_cast<FunctionDecl *>(decl.get());
                lowerFunction(module, *function);
            }
        }
        return module;
    }

    void Lowering::lowerFunction(
    mlir::ModuleOp module,
    const FunctionDecl &function) {

        std::cerr << "DEBUG 1: lowering function: "
                  << function.name << "\n";

        symbolTable.clear();

        std::cerr << "DEBUG 2: creating toy.func\n";

        auto func = toy::FuncOp::create(
            builder,
            builder.getUnknownLoc(),
            builder.getStringAttr(function.name));

        std::cerr << "DEBUG 3: toy.func created\n";

        auto &body = func.getBody();

        std::cerr << "DEBUG 4: got body\n";

        body.push_back(new mlir::Block());

        std::cerr << "DEBUG 5: added block\n";

        auto *entryBlock = &body.front();

        builder.setInsertionPointToEnd(entryBlock);

        std::cerr << "DEBUG 6: insertion point set\n";

        for (const auto &statement : function.body) {

            std::cerr << "DEBUG 7: lowering statement\n";

            lowerStatement(*statement);

            std::cerr << "DEBUG 8: statement lowered\n";
        }

        std::cerr << "DEBUG 9: all statements lowered\n";

        module.push_back(func);

        std::cerr << "DEBUG 10: function added to module\n";

        if (function.body.empty())
            throw std::runtime_error("Function contains no body");
    }
    /*
    void Lowering::lowerFunction(mlir::ModuleOp module,const FunctionDecl &function) {
        symbolTable.clear();

        //auto i32Type = mlir::IntegerType::get( &context, 32 );
        //auto funcType = mlir::FunctionType::get(&context,{},i32Type);
        //auto func = mlir::func::FuncOp::create(mlir::UnknownLoc::get(&context),function.name,funcType);
        //auto *entryBlock = func.addEntryBlock();


        auto func = toy::FuncOp::create(builder, builder.getUnknownLoc(), builder.getStringAttr(function.name));
        auto &body = func.getBody();
        body.push_back(new mlir::Block());
        auto *entryBlock = &body.front();
        builder.setInsertionPointToEnd(entryBlock);
        for (const auto &statement :function.body)
            lowerStatement(*statement);
        module.push_back(func);

        if (function.body.empty())
            throw std::runtime_error("Function contains no body");
    }
    */

    mlir::Value Lowering::lowerExpression(const Expression &expression) {

        if (expression.getKind() == ASTNodeKind::IntegerLiteral) {
            auto *integer = static_cast<const IntegerLiteral *>(&expression);

            auto type = mlir::IntegerType::get(&context, 32);
            auto value = mlir::IntegerAttr::get(type, integer->value);

            return toy::ConstantOp::create(
                builder,
                mlir::UnknownLoc::get(&context),
                type,
                value);
        }

        if (expression.getKind() == ASTNodeKind::BinaryExpression) {
            auto *binary = static_cast<const BinaryExpression *>(&expression);

            mlir::Value left = lowerExpression(*binary->left);
            mlir::Value right = lowerExpression(*binary->right);

            auto resultType = left.getType();

            switch (binary->op) {
                case '+':
                    return toy::AddOp::create(
                        builder,
                        mlir::UnknownLoc::get(&context),
                        resultType,
                        left,
                        right);

                case '-':
                    return toy::SubOp::create(
                        builder,
                        mlir::UnknownLoc::get(&context),
                        resultType,
                        left,
                        right);

                case '*':
                    return toy::MulOp::create(
                        builder,
                        mlir::UnknownLoc::get(&context),
                        resultType,
                        left,
                        right);

                case '/':
                    return toy::DivOp::create(
                        builder,
                        mlir::UnknownLoc::get(&context),
                        resultType,
                        left,
                        right);

                default:
                    throw std::runtime_error("Unsupported binary operator");
            }
        }

        if (expression.getKind() == ASTNodeKind::CallExpression) {
            auto *call = static_cast<const CallExpression *>(&expression);

            std::cerr << "DEBUG: lowering call to " << call->callee << "\n";

            auto callee = builder.getStringAttr(call->callee);

            std::cerr << "DEBUG: creating toy.call\n";

            auto callOp = toy::CallOp::create(
                builder,
                mlir::UnknownLoc::get(&context),
                builder.getI32Type(),
                callee);

            std::cerr << "DEBUG: toy.call created\n";

            return callOp.getResult();
        }

        if (expression.getKind() == ASTNodeKind::VariableReference) {
            auto *variable = static_cast<const VariableReference *>(&expression);

            auto it = symbolTable.find(variable->name);

            if (it == symbolTable.end()) {
                throw std::runtime_error(
                    "Undefined variable: " + variable->name);
            }

            auto location = builder.getUnknownLoc();

            return mlir::memref::LoadOp::create(
                builder,
                location,
                it->second);
        }
        throw std::runtime_error("Unsupported expression in lowering");
    }

    void Lowering::lowerStatement(const Statement &statement) {

        if (statement.getKind() == ASTNodeKind::VariableDecl) {
            auto *variableDecl =
                static_cast<const VariableDecl *>(&statement);

            auto location = builder.getUnknownLoc();
            auto i32Type = builder.getI32Type();
            auto memrefType = mlir::MemRefType::get({}, i32Type);

            auto memory =
                mlir::memref::AllocaOp::create(
                    builder,
                    location,
                    memrefType);

            mlir::Value value =
                lowerExpression(*variableDecl->initializer);

            mlir::memref::StoreOp::create(
                builder,
                location,
                value,
                memory);

            symbolTable[variableDecl->name] = memory;

            return;
        }

        if (statement.getKind() == ASTNodeKind::IfStmt) {
            auto *ifStmt = static_cast<const IfStmt *>(&statement);

            lowerIfStatement(*ifStmt);
            return;
        }

      
        if (statement.getKind() == ASTNodeKind::WhileStmt) {
            auto *whileStmt = static_cast<const WhileStmt *>(&statement);
            lowerWhileStatement(*whileStmt);
            return;
        }

        if (statement.getKind() == ASTNodeKind::ReturnStmt) {
            auto *returnStmt = static_cast<const ReturnStmt *>(&statement);

            mlir::Value value =lowerExpression(*returnStmt->value);

            if (!value)
                throw std::runtime_error("Failed to lower return expression");

            toy::ReturnOp::create(builder, mlir::UnknownLoc::get(&context), value);

            return;
        }
        throw std::runtime_error("Unsupported statement in lowering");
    }

    void Lowering::lowerIfStatement(const IfStmt &stmt) {

        mlir::Value condition = lowerExpression(*stmt.condition);
        auto ifOp = mlir::scf::IfOp::create(builder, builder.getUnknownLoc(), condition, false);
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