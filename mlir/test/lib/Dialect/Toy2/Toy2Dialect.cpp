#include "Toy2Dialect.h"
#include "mlir/IR/DialectRegistry.h"
#include "mlir/IR/OpImplementation.h"

using namespace mlir;
using namespace mlir::toy2;

#include "Toy2Dialect.cpp.inc"

#define GET_OP_CLASSES
#include "Toy2Ops.cpp.inc"

void Toy2Dialect::initialize() { addOperations<Const42Op, AddOp, ConstOp>(); }

::llvm::LogicalResult ConstOp::verify() {
  if (static_cast<int32_t>(getValue()) < 0)
    return emitOpError("value must not be negative");
  return success();
}

namespace test {
void registerToy2Dialect(DialectRegistry &registry) {
  registry.insert<Toy2Dialect>();
}
} // namespace test
