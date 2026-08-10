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

            mlir::LogicalResult matchAndRewrite(toy::ConstantOp op, mlir::PatternRewriter &rewriter) const override {

                auto value = op.getValueAttr();
                auto resultType = op.getResult().getType();

                rewriter.replaceOpWithNewOp<mlir::arith::ConstantOp>(op,resultType,value);

                return mlir::success();
            }
        };

        class AddToArithPattern : public mlir::OpRewritePattern<toy::AddOp> {

        public:
            using mlir::OpRewritePattern<toy::AddOp>::OpRewritePattern;

            mlir::LogicalResult matchAndRewrite(toy::AddOp op, mlir::PatternRewriter &rewriter) const override {

                mlir::Value lhs = op.getLhs();
                mlir::Value rhs = op.getRhs();

                rewriter.replaceOpWithNewOp<mlir::arith::AddIOp>(op, lhs, rhs);

                return mlir::success();
            }
        };

        class SubToArithPattern : public mlir::OpRewritePattern<toy::SubOp> {

        public:
            using mlir::OpRewritePattern<toy::SubOp>::OpRewritePattern;

            mlir::LogicalResult matchAndRewrite(toy::SubOp op, mlir::PatternRewriter &rewriter) const override {

                auto lhs = op.getLhs();
                auto rhs = op.getRhs();

                rewriter.replaceOpWithNewOp<mlir::arith::SubIOp>(op,lhs,rhs);

                return mlir::success();
            }
        };

        class MulToArithPattern : public mlir::OpRewritePattern<toy::MulOp> {

        public:
            using mlir::OpRewritePattern<toy::MulOp>::OpRewritePattern;

            mlir::LogicalResult matchAndRewrite(toy::MulOp op, mlir::PatternRewriter &rewriter) const override {

                auto lhs = op.getLhs();
                auto rhs = op.getRhs();

                rewriter.replaceOpWithNewOp<mlir::arith::MulIOp>( op,lhs,rhs);

                return mlir::success();
            }
        };

        class DivToArithPattern : public mlir::OpRewritePattern<toy::DivOp> {

        public:
            using mlir::OpRewritePattern<toy::DivOp>::OpRewritePattern;

            mlir::LogicalResult matchAndRewrite(toy::DivOp op, mlir::PatternRewriter &rewriter) const override {

                auto lhs = op.getLhs();
                auto rhs = op.getRhs();

                rewriter.replaceOpWithNewOp<mlir::arith::DivSIOp>(op,lhs,rhs);

                return mlir::success();
            }
        };

        class ConstantToArithPass : public mlir::PassWrapper<ConstantToArithPass,mlir::OperationPass<mlir::ModuleOp>> {

        public:
            void runOnOperation() override {

                mlir::MLIRContext *context = &getContext();
                mlir::RewritePatternSet patterns(context);
                patterns.add<ConstantToArithPattern>(context);
                patterns.add<AddToArithPattern>(context);
                patterns.add<SubToArithPattern>(context);
                patterns.add<MulToArithPattern>(context);
                patterns.add<DivToArithPattern>(context);

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