//
// Created by shevo on 8/9/26.
//

#include "toy/ToyDialect.h"
#include "toy/ToyOps.h"

using namespace mlir;

namespace toy {

    void ToyDialect::initialize() {
        addOperations<
            ConstantOp,
            AddOp,
            SubOp,
            MulOp,
            DivOp,
            ReturnOp
        >();
    }

} // namespace toy

#include "ToyDialect.cpp.inc"