//===- pmdemo.cpp - W1.5-a PassManager 编排练习 driver ------------------===//
// 用法: ./bin/pmdemo <input.mlir>
//===---------------------------------------------------------------------===//

#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Parser/Parser.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Transforms/Passes.h"
#include "llvm/Support/raw_ostream.h"

using namespace mlir;

int main(int argc, char **argv) {
  if (argc < 2) {
    llvm::errs() << "usage: pmdemo <input.mlir>\n";
    return 1;
  }

  MLIRContext ctx;
  ctx.loadDialect<func::FuncDialect, arith::ArithDialect>();

  OwningOpRef<ModuleOp> module = parseSourceFile<ModuleOp>(argv[1], &ctx);
  if (!module) {
    llvm::errs() << "parse failed\n";
    return 1;
  }

  PassManager pm(&ctx); // 顶层锚定 builtin.module

  //===-----------------------------------------------------------------===//
  //  <<<< 你写:把 3 个 pass 挂到你决定的层 >>>>
  //  可用工厂函数:
  //    createSymbolDCEPass()
  //    createCSEPass()
  //    createCanonicalizerPass()
  //  可用 API:
  //    pm.addPass( ... );
  //    pm.addNestedPass<func::FuncOp>( ... );
  //===-----------------------------------------------------------------===//
  pm.addPass(createSymbolDCEPass());
  pm.addNestedPass<func::FuncOp>(createCSEPass());
  pm.addNestedPass<func::FuncOp>(createCanonicalizerPass());

  if (failed(pm.run(*module))) {
    llvm::errs() << "pipeline failed\n";
    return 1;
  }
  module->print(llvm::outs());
  llvm::outs() << "\n";
  return 0;
}