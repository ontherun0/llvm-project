toy.func @main() {
  %0 = toy.constant dense<[[1.0, 2.0], [3.0, 4.0]]> : tensor<2x2xf64>
  %1 = toy.constant dense<[[5.0, 6.0], [7.0, 8.0]]> : tensor<2x2xf64>
  %2 = toy.sub %0, %1 : tensor<2x2xf64>
  toy.print %2 : tensor<2x2xf64>
  toy.return
}
