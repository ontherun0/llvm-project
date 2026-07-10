#ifndef MLIR_TEST_TOY2_TOY2DIALECT_H
#define MLIR_TEST_TOY2_TOY2DIALECT_H

#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"      // ← op 类需要:OpDefinition、RegionRange 等
#include "mlir/IR/Builders.h"          // ← 通常也要:build() 相关
#include "mlir/Interfaces/InferTypeOpInterface.h"  // 可选,先加着无害
#include "mlir/Bytecode/BytecodeOpInterface.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"

#include "Toy2Dialect.h.inc"

#define GET_OP_CLASSES
#include "Toy2Ops.h.inc"

#endif
