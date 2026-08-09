//
// Created by shevo on 8/9/26.
//
#include "toy/ConstantToArith.h"
#include "toy/ToyOps.h"

#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

#include <iostream>

namespace toy {

    namespace {

        class ConstantToArithPattern : public mlir::OpRewritePattern<toy::ConstantOp> {

        public:
            using mlir::OpRewritePattern<toy::ConstantOp>::OpRewritePattern;

            mlir::LogicalResult matchAndRewrite(toy::ConstantOp op,mlir::PatternRewriter &rewriter) const override {

                auto value = op.getValueAttr();
                auto resultType = op.getResult().getType();

                rewriter.replaceOpWithNewOp<mlir::arith::ConstantOp>(op,resultType,value);

                return mlir::success();
            }
        };

        class ConstantToArithPass : public mlir::PassWrapper<ConstantToArithPass,mlir::OperationPass<mlir::ModuleOp>> {

        public:
            void runOnOperation() override {

                mlir::MLIRContext *context = &getContext();
                mlir::RewritePatternSet patterns(context);
                patterns.add<ConstantToArithPattern>(context);

                if (mlir::failed(mlir::applyPatternsGreedily(getOperation(),std::move(patterns)))) {

                    signalPassFailure();
                }
            }
        };

    } // namespace

    std::unique_ptr<mlir::Pass> createConstantToArithPass() {
        return std::make_unique<ConstantToArithPass>();
    }

} // namespace toy