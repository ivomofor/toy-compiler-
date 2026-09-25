//
// Created by shevo on 8/9/26.
//
#include "toy/ToyOps.h"

#define GET_OP_CLASSES
#include "ToyOps.cpp.inc"

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