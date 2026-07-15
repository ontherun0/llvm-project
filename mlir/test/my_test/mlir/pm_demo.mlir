func.func @f(%a: i32) -> i32 {
  %0 = arith.addi %a, %a : i32
  %1 = arith.addi %a, %a : i32     // 与 %0 重复 → CSE 消掉
  %2 = arith.addi %0, %1 : i32
  return %2 : i32
}
func.func private @dead(%a: i32) -> i32 {   // private 且没人调 → symbol-dce 删掉整个函数
  return %a : i32
}
func.func @g(%a: i32) -> i32 {
  return %a : i32
}
