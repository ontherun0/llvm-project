module {
    func.func @scf_for_sum() -> i32 {
        %lb   = arith.constant 0   : index
        %ub   = arith.constant 101 : index   // 上界开区间,要含 100 就写 101
        %step = arith.constant 1   : index
        %init = arith.constant 0   : i32      // 累加初值

        %sum = scf.for %i = %lb to %ub step %step iter_args(%cur = %init) -> i32 {
            %i_i32 = arith.index_cast %i : index to i32
            %next = arith.addi %cur, %i_i32 : i32
            scf.yield %next : i32
        }

        return %sum : i32
    }
}
