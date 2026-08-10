include "mlir/IR/OpBase.td"
include "dialects/ToyDialect.td"

def Toy_ConstantOp : Toy_Op<"constant"> {
  let summary = "Toy constant operation";
  let arguments = (ins I32Attr:$value);
  let results = (outs I32);
}

def Toy_AddOp : Toy_Op<"add"> {
  let summary = "Toy integer addition";
  let arguments = (ins I32:$lhs, I32:$rhs);
  let results = (outs I32);
}

def Toy_SubOp : Toy_Op<"sub"> {
  let summary = "Toy integer subtraction";
  let arguments = (ins I32:$lhs, I32:$rhs);
  let results = (outs I32);
}

def Toy_MulOp : Toy_Op<"mul"> {
  let summary = "Toy integer multiplication";
  let arguments = (ins I32:$lhs, I32:$rhs);
  let results = (outs I32);
}

def Toy_DivOp : Toy_Op<"div"> {
  let summary = "Toy signed integer division";
  let arguments = (ins I32:$lhs, I32:$rhs);
  let results = (outs I32);
}

def Toy_ReturnOp : Toy_Op<"return"> {
  let summary = "Return from a Toy function";
  let arguments = (ins I32:$value);
  let traits = [Terminator];
}