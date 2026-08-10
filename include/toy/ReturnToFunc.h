//
// Created by shevo on 8/10/26.
//

#ifndef TOY_COMPILER_RETURNTOFUNC_H
#define TOY_COMPILER_RETURNTOFUNC_H

#include <memory>
#include "mlir/Pass/Pass.h"

namespace toy {

    std::unique_ptr<mlir::Pass> createReturnToFuncPass();

}

#endif //TOY_COMPILER_RETURNTOFUNC_H
