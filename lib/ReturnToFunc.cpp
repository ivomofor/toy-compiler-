#include "toy/ReturnToFunc.h"
#include "toy/ToyOps.h"

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

namespace toy {

    namespace {

        class ReturnToFuncPattern : public mlir::OpRewritePattern<toy::ReturnOp> {

        public:
            using mlir::OpRewritePattern<toy::ReturnOp>::OpRewritePattern;

            mlir::LogicalResult matchAndRewrite(toy::ReturnOp op, mlir::PatternRewriter &rewriter) const override {

                auto value = op.getValue();

                rewriter.replaceOpWithNewOp<mlir::func::ReturnOp>(op, value);

                return mlir::success();
            }
        };

        class ReturnToFuncPass
            : public mlir::PassWrapper<ReturnToFuncPass, mlir::OperationPass<mlir::ModuleOp>> {

        public:
            void runOnOperation() override {

                mlir::RewritePatternSet patterns(&getContext());
                patterns.add<ReturnToFuncPattern>(&getContext());

                if (mlir::failed(mlir::applyPatternsGreedily(getOperation(), std::move(patterns)))) {

                    signalPassFailure();
                }
            }
        };

    } // namespace

    std::unique_ptr<mlir::Pass> createReturnToFuncPass() {
        return std::make_unique<ReturnToFuncPass>();
    }

} // namespace toy