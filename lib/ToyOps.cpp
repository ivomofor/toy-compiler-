//
// Created by shevo on 8/9/26.
//
#include "toy/ToyOps.h"

#define GET_OP_CLASSES
#include "ToyOps.cpp.inc"

#define GET_OP_INTERFACE_DEFS
#include "ToyInterfaces.cpp.inc"

llvm::StringRef toy::AddOp::getToyName() {
    return getOperationName();
}

void toy::FuncOp::build(
    mlir::OpBuilder &builder,
    mlir::OperationState &state,
    llvm::StringRef name,
    llvm::ArrayRef<mlir::Attribute> parameters) {

    state.addAttribute(
        "sym_name",
        builder.getStringAttr(name));

    state.addAttribute(
        "parameters",
        builder.getArrayAttr(parameters));

    auto *region =
        state.addRegion();

    auto &entryBlock =
    region->emplaceBlock();

    auto i32Type =
        builder.getI32Type();

    for (auto parameter : parameters) {
        entryBlock.addArgument(
            i32Type,
            builder.getUnknownLoc());
    }
}

mlir::LogicalResult toy::FuncOp::verify() {
    if (getBody().empty()) {
        return emitOpError("function must have a body");
    }

    if (getSymName().empty()) {
        return emitOpError("function must have a name");
    }

    if (getParameters().size() != getBody().front().getNumArguments()) {
        return emitOpError(
            "number of parameters must match number of block arguments");
    }

    return mlir::success();
}

mlir::LogicalResult toy::TensorMatmulOp::verify() {
    auto lhsType = mlir::cast<mlir::RankedTensorType>(
        getLhs().getType());

    auto rhsType = mlir::cast<mlir::RankedTensorType>(
        getRhs().getType());

    auto resultType = mlir::cast<mlir::RankedTensorType>(
        getResult().getType());

    if (lhsType.getRank() != 2 || rhsType.getRank() != 2) {
        return emitOpError("expects rank-2 input tensors");
    }

    if (resultType.getRank() != 2) {
        return emitOpError("expects a rank-2 result tensor");
    }

    if (lhsType.getElementType() != rhsType.getElementType() ||
        lhsType.getElementType() != resultType.getElementType()) {
        return emitOpError("requires matching element types");
        }

    if (!lhsType.isDynamicDim(1) &&
        !rhsType.isDynamicDim(0) &&
        lhsType.getDimSize(1) != rhsType.getDimSize(0)) {
        return emitOpError(
            "inner dimensions of input tensors must match");
        }

    if (!lhsType.isDynamicDim(0) &&
        !resultType.isDynamicDim(0) &&
        lhsType.getDimSize(0) != resultType.getDimSize(0)) {
        return emitOpError(
            "result row count must match the left input");
        }

    if (!rhsType.isDynamicDim(1) &&
        !resultType.isDynamicDim(1) &&
        rhsType.getDimSize(1) != resultType.getDimSize(1)) {
        return emitOpError(
            "result column count must match the right input");
        }

    return mlir::success();
}