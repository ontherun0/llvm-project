// int min_i32(int a, int b) {
//   return a < b ? a : b;
// }
//
// int max_i32(int a, int b) {
//   return a > b ? a : b;
// }
//
// int clamp_i32(int x, int low, int high) {
//   return min_i32(max_i32(x, low), high);
// }


module {
  func.func @min_i32(%arg0: i32, %arg1: i32) -> i32 {
    %cond = arith.cmpi slt, %arg0, %arg1 : i32
    %r = scf.if %cond -> i32 {
        scf.yield %arg0 : i32
    } else {
        scf.yield %arg1 : i32
    }
    return %r : i32
  }

  func.func @max_i32(%arg0: i32, %arg1: i32) -> i32 {
    %cond = arith.cmpi sgt, %arg0, %arg1 : i32
    %r = scf.if %cond -> i32 {
        scf.yield %arg0 : i32
    } else {
        scf.yield %arg1 : i32
    }
    return %r : i32
  }

  func.func @clamp_i32(%arg0: i32, %arg1: i32, %arg2: i32) -> i32 {
    %max = func.call @max_i32(%arg0, %arg1) : (i32, i32) -> i32
    %clamp = func.call @min_i32(%max, %arg2) : (i32, i32) -> i32
    return %clamp : i32
  }
}
