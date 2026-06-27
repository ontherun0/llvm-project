module {
  func.func @sum_memref(%arg0: memref<?xi32>, %arg1: index) -> i32 {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c0_i32 = arith.constant 0 : i32
    cf.br ^bb1(%c0, %c0_i32 : index, i32)
  ^bb1(%0: index, %1: i32):  // 2 preds: ^bb0, ^bb2
    %2 = arith.cmpi slt, %0, %arg1 : index
    cf.cond_br %2, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %3 = memref.load %arg0[%0] : memref<?xi32>
    %4 = arith.addi %3, %1 : i32
    %5 = arith.addi %0, %c1 : index
    cf.br ^bb1(%5, %4 : index, i32)
  ^bb3:  // pred: ^bb1
    return %1 : i32
  }
}
