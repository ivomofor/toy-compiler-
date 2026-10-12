#include "toy/TensorMatmulToLinalg.h"
#include "toy/ToyOps.h"

#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

#include "llvm/ADT/SmallVector.h"

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

        llvm::SmallVector<mlir::Value> dynamicSizes;

        if (resultType.isDynamicDim(0)) {
            auto rows = mlir::tensor::DimOp::create(
                rewriter,
                op.getLoc(),
                op.getLhs(),
                0);

            dynamicSizes.push_back(rows);
        }

        if (resultType.isDynamicDim(1)) {
            auto columns = mlir::tensor::DimOp::create(
                rewriter,
                op.getLoc(),
                op.getRhs(),
                1);

            dynamicSizes.push_back(columns);
        }

        auto output = mlir::tensor::EmptyOp::create(
            rewriter,
            op.getLoc(),
            resultType.getShape(),
            resultType.getElementType(),
            dynamicSizes);

        auto zero = mlir::arith::ConstantOp::create(
            rewriter,
            op.getLoc(),
            resultType.getElementType(),
            rewriter.getFloatAttr(
                resultType.getElementType(),
                0.0));

        auto initializedOutput = mlir::linalg::FillOp::create(
            rewriter,
            op.getLoc(),
            mlir::TypeRange{resultType},
            mlir::ValueRange{zero},
            mlir::ValueRange{output});

        auto result = mlir::linalg::MatmulOp::create(
            rewriter,
            op.getLoc(),
            mlir::TypeRange{resultType},
            mlir::ValueRange{op.getLhs(), op.getRhs()},
            mlir::ValueRange{initializedOutput.getResult(0)});

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