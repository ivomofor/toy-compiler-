//
// Created by shevo on 8/9/26.
//

#ifndef TOY_DIALECT_H
#define TOY_DIALECT_H

#include "mlir/IR/Dialect.h"

#define GET_DIALECT_DECLS
#include "ToyDialect.h.inc"
#undef GET_DIALECT_DECLS

#endif
