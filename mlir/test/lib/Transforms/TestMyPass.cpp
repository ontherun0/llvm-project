//===- TestMyPass.cpp - Test my pass --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass tests the count addi utilities by implementing an example
// count addi pass.
//
//===----------------------------------------------------------------------===//

#include "mlir-c/Rewrite.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/IR/Attributes.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Operation.h"
#include "mlir/IR/Value.h"
#include "mlir/Pass/Pass.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/LogicalResult.h"
#include "llvm/Support/raw_ostream.h"
#include <cstddef>
#include <cstdint>
#include "mlir/IR/PatternMatch.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/Vector/IR/VectorOps.h"

using namespace mlir;

namespace {
static bool isIntegerZero(mlir::Value value) {
  // check if value is defined by a const op like: %c0 = arith.const 0 : i32.
  auto* defOp = value.getDefiningOp();
  auto constOp = llvm::dyn_cast_or_null<mlir::arith::ConstantOp>(defOp);
  if (!constOp)
    return false;
  auto constValue = constOp.getValue();
  auto intAttr = llvm::dyn_cast<mlir::IntegerAttr>(constValue);
  if (!intAttr)
    return false;
  if (!intAttr.getValue().isZero())
    return false;
  return true;
}

static bool areInversePermutations(ArrayRef<int64_t> inner,
                                   ArrayRef<int64_t> outer) {
  if (inner.size() != outer.size())
    return false;
  for (size_t i = 0; i < inner.size(); ++i) {
    if (inner[outer[i]] != static_cast<int64_t>(i))
      return false;
  }
  return true;
}

/// Write pass with pattern rewrite.
/// Delete the unnecessary arith.addi op if lhs is 0.
///   arith.addi 0, x  ->  x
struct SimplifyAddIWithZero : public OpRewritePattern<arith::AddIOp> {
  using OpRewritePattern<arith::AddIOp>::OpRewritePattern;
  LogicalResult matchAndRewrite(arith::AddIOp addOp,
                                PatternRewriter &rewriter) const override {
    Value lhs = addOp.getLhs();
    Value rhs = addOp.getRhs();
    Operation *lhsDef = lhs.getDefiningOp();
    auto constOp = dyn_cast_or_null<arith::ConstantOp>(lhsDef);
    if (!constOp)
      return failure();
    auto value = constOp.getValue();
    auto intAttr = dyn_cast<IntegerAttr>(value);
    if (!intAttr)
      return failure();
    if (!intAttr.getValue().isZero())
      return failure();
    rewriter.replaceOp(addOp, rhs);
    return success();
  }
};

struct TestSimplifyAddIWithZeroPass
    : public PassWrapper<TestSimplifyAddIWithZeroPass,
                         OperationPass<func::FuncOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(TestSimplifyAddIWithZeroPass)
  /// Get the command-line argument of the test pass.
  StringRef getArgument() const final { return "test-simplify-addi-with-zero"; }
  /// Get the description of the test pass.
  StringRef getDescription() const final {
    return "Test simplify addi with zero pass";
  }

  void runOnOperation() override {
    RewritePatternSet patterns(&getContext());
    patterns.add<SimplifyAddIWithZero>(&getContext());
    if (failed(applyPatternsGreedily(getOperation(),
                                            std::move(patterns)))) {
      signalPassFailure();
    }
  }
};

/// Delete the unnecessary arith.addi op if lhs is 0 or rhs is 0.
///   arith.addi 0, x  ->  x
struct SeniorSimplifyAddIWithZero : public OpRewritePattern<arith::AddIOp> {
  using OpRewritePattern<arith::AddIOp>::OpRewritePattern;
  LogicalResult matchAndRewrite(arith::AddIOp addOp,
                                PatternRewriter &rewriter) const override {
    auto lhs = addOp.getLhs();
    auto rhs = addOp.getRhs();
    if (isIntegerZero(lhs)) {
      // replace addi with rhs
      rewriter.replaceOp(addOp, rhs);
      return success();
    }
    if (isIntegerZero(rhs)) {
      // replace addi with lhs
      rewriter.replaceOp(addOp, lhs);
      return success();
    }
    return failure();
  }
};

struct TestSeniorSimplifyAddIWithZeroPass
    : public PassWrapper<TestSeniorSimplifyAddIWithZeroPass,
                         OperationPass<func::FuncOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(TestSeniorSimplifyAddIWithZeroPass)
  /// Get the command-line argument of the test pass.
  StringRef getArgument() const final { return "test-senior-simplify-addi-with-zero"; }
  /// Get the description of the test pass.
  StringRef getDescription() const final {
    return "Test senior simplify addi with zero pass";
  }

  void runOnOperation() override {
    RewritePatternSet patterns(&getContext());
    patterns.add<SeniorSimplifyAddIWithZero>(&getContext());
    if (failed(applyPatternsGreedily(getOperation(),
                                            std::move(patterns)))) {
      signalPassFailure();
    }
  }
};

/// Raw pass.
struct TestCountAddIPass
    : public PassWrapper<TestCountAddIPass, OperationPass<ModuleOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(TestCountAddIPass)

  /// Get the command-line argument of the test pass.
  StringRef getArgument() const final { return "test-count-addi"; }
  /// Get the description of the test pass.
  StringRef getDescription() const final {
    return "Test count addi pass";
  }

  /// Runs the pass on the module.
  // void runOnOperation() override {
  //   int count = 0;
  //   getOperation()->walk([&](Operation *op) {
  //     // TODO: 判断是不是 arith.addi
  //     if (op->getName().getStringRef() == "arith.addi") {
  //       ++count;
  //     }
  //   });
  //   llvm::outs() << "arith.addi count = " << count << "\n";
  // }

  /// Runs the pass on the func.
  // void runOnOperation() override {
  //   getOperation()->walk([&](func::FuncOp funcOp) {
  //       int count = 0;
  //       funcOp.walk([&](Operation* op) {
  //           if (op->getName().getStringRef() == "arith.addi") {
  //               ++count;
  //           }
  //       });
  //       llvm::outs() << funcOp.getName() << " has " << count << " arith.addi." << "\n";
  //   });
  // }
  
  /// Delete the unnecessary arith.addi op if one of the operands is 0
  void runOnOperation() override {
    SmallVector<std::pair<arith::AddIOp, Value>> rewrites;
    getOperation()->walk([&](arith::AddIOp addOp) {
      Value lhs = addOp.getLhs();
      Value rhs = addOp.getRhs();
      Operation* lhsDef = lhs.getDefiningOp();
      arith::ConstantOp constOp = dyn_cast_or_null<arith::ConstantOp>(lhsDef);
      if (!constOp) return;
      auto value = constOp.getValue();
      auto intAttr = dyn_cast<IntegerAttr>(value);
      if (!intAttr) return;
      if (intAttr.getValue().isZero()) {
        rewrites.push_back({addOp, rhs});
      }
    });
    for (auto [addOp, replacement] : rewrites) {
      addOp.getResult().replaceAllUsesWith(replacement);
      addOp.erase();
    }
  }
};

/// Write pass with pattern rewrite.
/// Simplify redundant transpose.
///   vector.transpose(vector.transpose(x)) -> x
struct SimplifyRedundantTranspose : public OpRewritePattern<vector::TransposeOp> {
  using OpRewritePattern<vector::TransposeOp>::OpRewritePattern;
  LogicalResult matchAndRewrite(vector::TransposeOp transposeOp,
                                PatternRewriter &rewriter) const override {
    auto transposeInput = transposeOp.getVector();
    vector::TransposeOp transposeInputOp = transposeInput.getDefiningOp<vector::TransposeOp>();
    if (!transposeInputOp ||
        !areInversePermutations(transposeInputOp.getPermutation(),
                                transposeOp.getPermutation())) {
      return failure();
    }
    rewriter.replaceOp(transposeOp, transposeInputOp.getVector());
    return success();
  }
};

struct TestSimplifyRedundantTransposePass
    : public PassWrapper<TestSimplifyRedundantTransposePass,
                         OperationPass<func::FuncOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(TestSimplifyRedundantTransposePass)
  /// Get the command-line argument of the test pass.
  StringRef getArgument() const final { return "test-simplify-redundant-transpose"; }
  /// Get the description of the test pass.
  StringRef getDescription() const final {
    return "Test simplify redundant transpose pass";
  }

  void runOnOperation() override {
    RewritePatternSet patterns(&getContext());
    patterns.add<SimplifyRedundantTranspose>(&getContext());
    if (failed(applyPatternsGreedily(getOperation(),
                                            std::move(patterns)))) {
      signalPassFailure();
    }
  }
};
} // end anonymous namespace

namespace mlir {
namespace test {
void registerTestCountAddI() {
  PassRegistration<TestCountAddIPass>();
}

void registerTestSimplifyAddIWithZero() {
  PassRegistration<TestSimplifyAddIWithZeroPass>();
}

void registerTestSeniorSimplifyAddIWithZero() {
  PassRegistration<TestSeniorSimplifyAddIWithZeroPass>();
}

void registerTestSimplifyRedundantTranspose() {
  PassRegistration<TestSimplifyRedundantTransposePass>();
}
} // end namespace test
} // end namespace mlir
