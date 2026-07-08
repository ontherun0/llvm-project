//===- TestArithIndexCastLowering.cpp - Test LLVM Conversion of IndexCastOp
//--------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "mlir/Conversion/ArithToLLVM/ArithToLLVM.h"
#include "mlir/Conversion/LLVMCommon/ConversionTarget.h"
#include "mlir/Conversion/LLVMCommon/Pattern.h"
#include "mlir/Conversion/LLVMCommon/TypeConverter.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/DialectConversion.h"

using namespace mlir;

namespace {

class MyIndexCastLowering : public ConvertOpToLLVMPattern<arith::IndexCastOp> {
public:
  using ConvertOpToLLVMPattern<arith::IndexCastOp>::ConvertOpToLLVMPattern;

  LogicalResult
  matchAndRewrite(arith::IndexCastOp op, OpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    // 1. 取 srcW、dstW 两个位宽，取 Type resultTy = op.getType()。
    // 2. 三路分支:
    //  2.1 srcW > dstW → rewriter.replaceOpWithNewOp<LLVM::TruncOp>(op, resultTy, adaptor.getIn())
    //  2.2 srcW < dstW → rewriter.replaceOpWithNewOp<LLVM::SExtOp>(op, resultTy, adaptor.getIn())
    //  2.3 相等 → rewriter.replaceOp(op, adaptor.getIn())
    // 3. return success();
    auto resultTy = op.getType();
    auto srcW = adaptor.getIn().getType().getIntOrFloatBitWidth();
    auto dstW = resultTy.getIntOrFloatBitWidth();
    if (srcW > dstW) {
      rewriter.replaceOpWithNewOp<LLVM::TruncOp>(op, resultTy, adaptor.getIn());
    } else if (srcW < dstW) {
      rewriter.replaceOpWithNewOp<LLVM::SExtOp>(op, resultTy, adaptor.getIn());
    } else {
      rewriter.replaceOp(op, adaptor.getIn());
    }
    return success();
  }
};

struct TestArithIndexCastLoweringPass
    : public PassWrapper<TestArithIndexCastLoweringPass,
                         OperationPass<ModuleOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(TestArithIndexCastLoweringPass)

  void getDependentDialects(DialectRegistry &registry) const final {
    registry.insert<LLVM::LLVMDialect>();
  }
  StringRef getArgument() const final {
    return "test-arith-index-cast-lowering";
  }
  StringRef getDescription() const final {
    return "Tests conversion of `arith.index_cast` to `llvm.cast_xxx` in "
           "presence of custom types";
  }

  void runOnOperation() override {
    LLVMTypeConverter converter(&getContext());
    RewritePatternSet patterns(&getContext());
    LLVMConversionTarget target(getContext());
    target.addIllegalDialect<arith::ArithDialect>();
    // NOTE: Unnecessary, see
    // mlir/lib/Conversion/LLVMCommon/ConversionTarget.cpp:16.
    // target.addLegalDialect<LLVM::LLVMDialect>();
    patterns.add<MyIndexCastLowering>(converter);
    if (failed(applyPartialConversion(getOperation(), target,
                                      std::move(patterns))))
      signalPassFailure();
  }
};

} // namespace

namespace mlir {
namespace test {
void registerTestArithIndexCastLowering() {
  PassRegistration<TestArithIndexCastLoweringPass>();
}
} // namespace test
} // namespace mlir
