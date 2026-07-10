func.func @add_two(%a: tensor<4xf32>, %b: tensor<4xf32>) -> tensor<4xf32> {
  // 1. 造一个输出 tensor,给 linalg.add 当 outs 目标(想想哪个 op 能凭空造一个 tensor<4xf32>)
  %init = tensor.empty() : tensor<4xf32>

  // 2. 调 linalg.add:两个输入走 ins(...),上面的 %init 走 outs(...),它返回结果 tensor
  //    语法结构去文档/test 里抄:linalg.add ins(...) outs(...) -> ...
  %result = linalg.add ins(%a, %b : tensor<4xf32>, tensor<4xf32>)
                        outs(%init : tensor<4xf32>) -> tensor<4xf32>

  return %result : tensor<4xf32>
}