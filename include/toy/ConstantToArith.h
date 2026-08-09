//
// Created by shevo on 8/9/26.
//

#ifndef TOY_COMPILER_CONSTANTTOARITH_H
#define TOY_COMPILER_CONSTANTTOARITH_H

#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"

namespace toy {

    std::unique_ptr<mlir::Pass> createConstantToArithPass();

} // namespace toy

#endif //TOY_COMPILER_CONSTANTTOARITH_H
