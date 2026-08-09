//
// Created by shevo on 8/9/26.
//

#ifndef TOY_COMPILER_TOYOPS_H
#define TOY_COMPILER_TOYOPS_H

#include "mlir/IR/OpDefinition.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/Bytecode/BytecodeOpInterface.h"

#define GET_OP_CLASSES
#include "ToyOps.h.inc"

#endif // TOY_COMPILER_TOYOPS_H

