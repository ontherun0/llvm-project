// int sum(int *A, int n) {
//   int s = 0;
//   for (int i = 0; i < n; i++)
//     s += A[i];
//   return s;
// }


module {
    func.func @sum_memref(%A : memref<?xi32>, %n : index) -> i32 {
        %start_index = arith.constant 0 : index
        %step = arith.constant 1 : index
        %zero = arith.constant 0 : i32

        %sum = scf.for %i = %start_index to %n step %step iter_args(%acc = %zero) -> i32 {
            %cur = memref.load %A[%i] : memref<?xi32>
            %next = arith.addi %cur, %acc : i32
            scf.yield %next : i32
        }

        return %sum : i32
    }
}
