module {
  llvm.func @scf_for_sum() -> i32 {
    %0 = llvm.mlir.constant(0 : index) : i64
    %1 = llvm.mlir.constant(101 : index) : i64
    %2 = llvm.mlir.constant(1 : index) : i64
    %3 = llvm.mlir.constant(0 : i32) : i32
    llvm.br ^bb1(%0, %3 : i64, i32)
  ^bb1(%4: i64, %5: i32):  // 2 preds: ^bb0, ^bb2
    %6 = llvm.icmp "slt" %4, %1 : i64
    llvm.cond_br %6, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %7 = llvm.trunc %4 : i64 to i32
    %8 = llvm.add %5, %7 : i32
    %9 = llvm.add %4, %2 : i64
    llvm.br ^bb1(%9, %8 : i64, i32)
  ^bb3:  // pred: ^bb1
    llvm.return %5 : i32
  }
}

