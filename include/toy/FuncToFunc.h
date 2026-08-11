#ifndef TOY_COMPILER_FUNCTOFUNC_H
#define TOY_COMPILER_FUNCTOFUNC_H

#include <memory>
#include "mlir/Pass/Pass.h"

namespace toy {

    std::unique_ptr<mlir::Pass> createFuncToFuncPass();

}

#endif