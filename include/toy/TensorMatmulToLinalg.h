//
// Created by shevo on 10/6/26.
//

#ifndef TOY_COMPILER_TENSORMATMULTOLTOLINALG_H
#define TOY_COMPILER_TENSORMATMULTOLTOLINALG_H

#include "mlir/Pass/Pass.h"

namespace toy {

    std::unique_ptr<mlir::Pass> createTensorMatmulToLinalgPass();

}

#endif