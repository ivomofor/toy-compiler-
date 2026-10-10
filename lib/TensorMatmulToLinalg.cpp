//
// Created by shevo on 10/6/26.
//

#include "toy/TensorMatmulToLinalg.h"
#include "toy/ToyOps.h"

#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

namespace {

    class TensorMatmulToLinalgPattern
        : public mlir::OpRewritePattern<toy::TensorMatmulOp> {
    public:
        using OpRewritePattern<toy::TensorMatmulOp>::OpRewritePattern;

        mlir::LogicalResult matchAndRewrite(
            toy::TensorMatmulOp op,
            mlir::PatternRewriter &rewriter) const override {

            auto resultType = mlir::cast<mlir::RankedTensorType>(
                op.getResult().getType());

            auto output = mlir::tensor::EmptyOp::create(
                rewriter,
                op.getLoc(),
                resultType.getShape(),
                resultType.getElementType());

            auto zero = mlir::arith::ConstantOp::create(
                rewriter,
                op.getLoc(),
                resultType.getElementType(),
                rewriter.getFloatAttr(resultType.getElementType(), 0.0));

            auto initializedOutput = mlir::linalg::FillOp::create(
                rewriter,
                op.getLoc(),
                mlir::TypeRange{resultType},
                mlir::ValueRange{zero},
                mlir::ValueRange{output});

            auto result = mlir::linalg::MatmulOp::create(
                rewriter,
                op.getLoc(),
                op.getResult().getType(),
                {op.getLhs(), op.getRhs()},
                {initializedOutput.getResult(0)});

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

}

std::unique_ptr<mlir::Pass>
toy::createTensorMatmulToLinalgPass() {
    return std::make_unique<TensorMatmulToLinalgPass>();
}