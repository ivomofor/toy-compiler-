//
// Created by shevo on 8/9/26.
//
#include "toy/ToyOps.h"

#define GET_OP_CLASSES
#include "ToyOps.cpp.inc"

mlir::LogicalResult toy::FuncOp::verify() {
    if (getBody().empty()) {
        return emitOpError("function must have a body");
    }

    return mlir::success();
}