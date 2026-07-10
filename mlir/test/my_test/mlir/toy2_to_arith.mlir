// RUN: mlir-opt --test-toy2-to-arith %s | FileCheck %s

// CHECK-LABEL: func @f
// CHECK-SAME:  (%[[A:.*]]: i32, %[[B:.*]]: i32) -> i32
func.func @f(%a: i32, %b: i32) -> i32 {
  // CHECK: %[[R:.*]] = arith.addi %[[A]], %[[B]] : i32
  %0 = toy2.add %a, %b : i32
  // CHECK: return %[[R]] : i32
  return %0 : i32
}
