toy.func @main() {
  %0 = toy.constant dense<[[1.0, 2.0], [3.0, 4.0]]> : tensor<2x2xf64>
  %1 = toy.square %0 : (tensor<2x2xf64>) -> tensor<2x2xf64>
  toy.print %1 : tensor<2x2xf64>
  toy.return
}
