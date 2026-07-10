#ifndef TOY2_DIALECT
#define TOY2_DIALECT

include "mlir/IR/OpBase.td"
include "mlir/IR/CommonTypeConstraints.td"
include "mlir/Interfaces/InferTypeOpInterface.td"

def Toy2_Dialect : Dialect {
  let name = "toy2";
  let cppNamespace = "::mlir::toy2";
  let summary = "A minimal dialect for learning ODS.";
}

class Toy2_Op<string mnemonic, list<Trait> traits = []> :
    Op<Toy2_Dialect, mnemonic, traits>;

def Const42Op : Toy2_Op<"const42"> {
  let summary = "produce the constant 42";
  let results = (outs I32:$value);
  let assemblyFormat = "attr-dict `:` type(results)";
}

def AddOp : Toy2_Op<"add", [SameOperandsAndResultType]> {
  let summary = "addition of two I32";
  let arguments = (ins I32:$lhs, I32:$rhs);
  let results = (outs I32:$result);
  let assemblyFormat = "$lhs`,` $rhs attr-dict `:` type($result)";
}

def ConstOp : Toy2_Op<"const"> {
  let summary = "produce the constant";
  let arguments = (ins I32Attr:$value);
  let results = (outs I32:$result);
  let assemblyFormat = "$value attr-dict `:` type(results)";
  let hasVerifier = 1;
}

#endif // TOY2_DIALECT
