//
// Created by shevo on 10/6/26.
//

#include "toy/TensorMatmulToLinalg.h"
#include "toy/ToyOps.h"

#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"

namespace {
    class TensorMatmulToLinalgPattern
        : public mlir::OpRewritePattern<toy::TensorMatmulOp> {
    public:
        using OpRewritePattern<toy::TensorMatmulOp>::OpRewritePattern;

        mlir::LogicalResult matchAndRewrite(
            toy::TensorMatmulOp op,
            mlir::PatternRewriter &rewriter) const override {

            auto result = rewriter.create<mlir::linalg::MatmulOp>(
                op.getLoc(),
                op.getResult().getType(),
                op.getLhs(),
                op.getRhs());

            rewriter.replaceOp(op, result);

            return mlir::success();
        }
    };

    struct TensorMatmulToLinalgPass
       : public mlir::PassWrapper<
             TensorMatmulToLinalgPass,
             mlir::OperationPass<mlir::ModuleOp>> {

        void runOnOperation() override {
            mlir::RewritePatternSet patterns(&getContext());

            patterns.add<TensorMatmulToLinalgPattern>(
                &getContext());

            if (mlir::failed(
                    mlir::applyPatternsGreedily(
                        getOperation(),
                        std::move(patterns)))) {
                signalPassFailure();
                        }
        }
    };

    std::unique_ptr<mlir::Pass> toy::createTensorMatmulToLinalgPass() {
        return std::make_unique<TensorMatmulToLinalgPass>();
    }
}
