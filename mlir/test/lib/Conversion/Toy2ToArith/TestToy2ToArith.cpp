//===- TestToy2ToArith.cpp -
//--------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Toy2Dialect.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/DialectConversion.h"

using namespace mlir;

namespace {

class AddOpLowering : public OpConversionPattern<mlir::toy2::AddOp> {
public:
  using OpConversionPattern<mlir::toy2::AddOp>::OpConversionPattern;

  LogicalResult
  matchAndRewrite(mlir::toy2::AddOp op, OpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    auto targetLhs = adaptor.getLhs();
    auto targetRhs = adaptor.getRhs();
    auto resultType = getTypeConverter()->convertType(op.getResult().getType());
    if (!resultType)
      return failure();
    rewriter.replaceOpWithNewOp<arith::AddIOp>(op, resultType, targetLhs,
                                               targetRhs);
    return success();
  }
};

struct TestToy2ToArithPass
    : public PassWrapper<TestToy2ToArithPass, OperationPass<ModuleOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(TestToy2ToArithPass)

  StringRef getArgument() const final { return "test-toy2-to-arith"; }
  StringRef getDescription() const final {
    return "Tests conversion of `toy2` to `arith`";
  }

  void runOnOperation() override {
    TypeConverter converter;
    converter.addConversion([](IntegerType type) { return type; });
    ConversionTarget target(getContext());
    target.addLegalDialect<arith::ArithDialect>();
    target.addLegalDialect<func::FuncDialect>();
    target.addIllegalDialect<mlir::toy2::Toy2Dialect>();
    RewritePatternSet patterns(&getContext());
    patterns.add<AddOpLowering>(converter, &getContext());
    if (failed(applyPartialConversion(getOperation(), target,
                                      std::move(patterns))))
      signalPassFailure();
  }
};

} // namespace

namespace mlir {
namespace test {
void registerTestToy2ToArithPass() { PassRegistration<TestToy2ToArithPass>(); }
} // namespace test
} // namespace mlir