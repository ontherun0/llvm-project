// RUN: mlir-opt %s | mlir-opt | FileCheck %s

func.func @f() {
  // CHECK: %[[A:.*]] = toy2.const42 : i32
  %0 = toy2.const42 : i32
  // CHECK: %[[B:.*]] = toy2.const 42 : i32
  // %1 = toy2.const -5 : i32
  %1 = toy2.const 42 : i32
  // CHECK: %{{.*}} = toy2.add %[[A]], %[[B]] : i32
  %2 = toy2.add %0, %1 : i32
  return
}