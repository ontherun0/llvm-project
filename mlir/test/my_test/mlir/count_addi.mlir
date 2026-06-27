module {
  func.func @foo(%a: i32, %b: i32, %c: i32) -> i32 {
    %0 = arith.addi %a, %b : i32
    %1 = arith.addi %0, %c : i32
    return %1 : i32
  }

  func.func @bar(%x: i32, %y: i32) -> i32 {
    %0 = arith.muli %x, %y : i32
    return %0 : i32
  }
}
