// void saxpy_i32(int a, int *x, int *y, int *out, int n) {
//   for (int i = 0; i < n; i++) {
//     out[i] = a * x[i] + y[i];
//   }
// }


module {
    func.func @saxpy_i32(%a : i32, %x : memref<?xi32>, %y : memref<?xi32>, %out : memref<?xi32>, %n : index) {
        %start_idx = arith.constant 0 : index
        %step = arith.constant 1 : index

        scf.for %i = %start_idx to %n step %step {
            %cur_x = memref.load %x[%i] : memref<?xi32>
            %cur_x_mul = arith.muli %a, %cur_x : i32
            %cur_y = memref.load %y[%i] : memref<?xi32>
            %cur = arith.addi %cur_x_mul, %cur_y : i32
            memref.store %cur, %out[%i] : memref<?xi32>
        }

        return
    }
}
