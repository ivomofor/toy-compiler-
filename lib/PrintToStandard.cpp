//
// Created by shevo on 9/17/26.
//

#include "toy/PrintToStandard.h"
#include "toy/ToyOps.h"

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

namespace toy {

namespace {

class PrintToStandardPattern
    : public mlir::OpRewritePattern<toy::PrintOp> {

public:
    using mlir::OpRewritePattern<toy::PrintOp>::OpRewritePattern;

    mlir::LogicalResult matchAndRewrite(
        toy::PrintOp op,
        mlir::PatternRewriter &rewriter) const override {

        auto value = op.getValue();

        mlir::func::CallOp::create(
            rewriter,
            op.getLoc(),
            mlir::TypeRange{},
            "toy_print",
            mlir::ValueRange{value});

        rewriter.eraseOp(op);

        return mlir::success();
    }
};

class PrintToStandardPass
    : public mlir::PassWrapper<
          PrintToStandardPass,
          mlir::OperationPass<mlir::ModuleOp>> {

public:
    void runOnOperation() override {

        mlir::ModuleOp module = getOperation();

        mlir::MLIRContext *context = &getContext();

        auto i32Type = mlir::IntegerType::get(context, 32);

        auto functionType =
            mlir::FunctionType::get(
                context,
                {i32Type},
                {});

        if (!module.lookupSymbol<mlir::func::FuncOp>("toy_print")) {

            auto printFunction =
                mlir::func::FuncOp::create(
                    module.getLoc(),
                    "toy_print",
                    functionType);

            printFunction.setPrivate();

            module.push_back(printFunction);
        }

        mlir::RewritePatternSet patterns(context);

        patterns.add<PrintToStandardPattern>(context);

        if (mlir::failed(
                mlir::applyPatternsGreedily(
                    module,
                    std::move(patterns)))) {

            signalPassFailure();
        }
    }
};

}

std::unique_ptr<mlir::Pass> createPrintToStandardPass() {
    return std::make_unique<PrintToStandardPass>();
}

}