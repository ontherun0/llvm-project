# Exclude tests from libMLIR.so
add_mlir_library(MLIRTestArithToLLVM
  TestArithIndexCastLowering.cpp

  EXCLUDE_FROM_LIBMLIR
  )
mlir_target_link_libraries(MLIRTestArithToLLVM PUBLIC
  MLIRArithToLLVM
  MLIRArithDialect
  MLIRLLVMCommonConversion
  MLIRLLVMDialect
  MLIRPass
  MLIRTransforms
  )
