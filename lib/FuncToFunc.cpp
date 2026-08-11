#include "toy/FuncToFunc.h"
#include "toy/ToyOps.h"

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

namespace toy {

    namespace {

        class FuncToFuncPattern : public mlir::OpRewritePattern<toy::FuncOp> {

        public:
            using mlir::OpRewritePattern<toy::FuncOp>::OpRewritePattern;

            mlir::LogicalResult matchAndRewrite(toy::FuncOp op, mlir::PatternRewriter &rewriter) const override {

                auto name = op.getSymName();

                auto i32Type = rewriter.getI32Type();
                auto funcType = rewriter.getFunctionType({}, i32Type);
                auto newFunc = mlir::func::FuncOp::create(rewriter , op.getLoc(), name, funcType);
                //rewriter.inlineRegionBefore(op.getBody(), newFunc.getBody(), newFunc.getBody().end());
                //auto &oldBlock = op.getBody().front();
                //auto &newBlock = newFunc.getBody().front();
                //rewriter.mergeBlocks(&oldBlock, &newBlock);
                newFunc.getBody().takeBody(op.getBody());
                rewriter.replaceOp(op, newFunc);

                return mlir::success();
            }
        };


        class FuncToFuncPass : public mlir::PassWrapper<FuncToFuncPass, mlir::OperationPass<mlir::ModuleOp>> {

        public:
            void runOnOperation() override {
                mlir::RewritePatternSet patterns(&getContext());

                patterns.add<FuncToFuncPattern>(&getContext());

                if (mlir::failed(mlir::applyPatternsGreedily(getOperation(), std::move(patterns)))) {
                    signalPassFailure();
                }
            }
        };

    } // namespace

    std::unique_ptr<mlir::Pass> createFuncToFuncPass() {
        return std::make_unique<FuncToFuncPass>();
    }

} // namespace toy