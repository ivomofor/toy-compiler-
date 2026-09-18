//
// Created by shevo on 9/17/26.
//

#ifndef TOY_COMPILER_PRINTTOSTANDARD_H
#define TOY_COMPILER_PRINTTOSTANDARD_H

#include "mlir/Pass/Pass.h"

#include <memory>

namespace toy {

    std::unique_ptr<mlir::Pass> createPrintToStandardPass();

}

#endif