module {
  func.func @foo(%x: i32, %y: i32) -> i32 {
    %c0 = arith.constant 0 : i32
    %0 = arith.addi %c0, %x : i32
    %1 = arith.addi %0, %y : i32
    return %1 : i32
  }
}
