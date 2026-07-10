set(LLVM_TARGET_DEFINITIONS Toy2Dialect.td)
mlir_tablegen(Toy2Dialect.h.inc -gen-dialect-decls -dialect=toy2)
mlir_tablegen(Toy2Dialect.cpp.inc -gen-dialect-defs -dialect=toy2)
mlir_tablegen(Toy2Ops.h.inc -gen-op-decls -dialect=toy2)
mlir_tablegen(Toy2Ops.cpp.inc -gen-op-defs -dialect=toy2)
add_public_tablegen_target(MLIRToy2IncGen)

add_mlir_library(MLIRToy2
  Toy2Dialect.cpp

  DEPENDS
  MLIRToy2IncGen

  LINK_LIBS PUBLIC
  MLIRIR
  MLIRSupport
  )

target_include_directories(MLIRToy2 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  )
