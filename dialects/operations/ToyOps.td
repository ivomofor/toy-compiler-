include "mlir/IR/OpBase.td"
include "dialects/ToyDialect.td"

def Toy_ConstantOp : Toy_Op<"constant"> {
  let summary = "Toy constant operation";
  let arguments = (ins I64Attr:$value);
  let results = (outs I32);
}