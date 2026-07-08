func.func @t(%arg0: index) -> i128 {
  %0 = arith.index_cast %arg0 : index to i128
  return %0 : i128
}
