add_llvm_executable(pmdemo pmdemo.cpp)
llvm_update_compile_flags(pmdemo)
target_link_libraries(pmdemo PRIVATE
  MLIRIR
  MLIRParser
  MLIRPass
  MLIRTransforms
  MLIRFuncDialect
  MLIRArithDialect)
