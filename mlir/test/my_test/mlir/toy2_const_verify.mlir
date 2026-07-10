// RUN: mlir-opt %s -verify-diagnostics

func.func @bad() -> i32 {
  // expected-error @+1 {{must not be negative}}
  %0 = "toy2.const"() {value = 5 : i32} : () -> i32
  return %0 : i32
}
