// int select_i32_cf(int x, int y, bool cond) {
//   if (cond)
//     return x;
//   else
//     return y;
// }


module {
  func.func @select_i32_cf(%x: i32, %y: i32, %cond: i1) -> i32 {
    cf.cond_br %cond, ^then, ^else
    
  ^then:
    cf.br ^merge(%x : i32)
        
  ^else:
    cf.br ^merge(%y : i32)

  ^merge(%r : i32):
    return %r : i32
  }
}
