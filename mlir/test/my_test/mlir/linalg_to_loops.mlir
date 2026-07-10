func.func @add(%a: tensor<4xf32>, %b: tensor<4xf32>) -> tensor<4xf32> {
  %e = tensor.empty() : tensor<4xf32>
  %r = linalg.add ins(%a, %b : tensor<4xf32>, tensor<4xf32>)
                  outs(%e : tensor<4xf32>) -> tensor<4xf32>
  return %r : tensor<4xf32>
}