add_mlir_library(MLIRToy2ToArithTestPasses
  TestToy2ToArith.cpp

  EXCLUDE_FROM_LIBMLIR

  LINK_LIBS PUBLIC
  MLIRArithDialect
  MLIRPass
  MLIRTransforms
  MLIRToy2      # 第 4 步写 pattern 引用 toy2::AddOp 时再解开，填你 W2 的实际库名
  )
