include "mlir/IR/DialectBase.td"
include "mlir/IR/OpBase.td"

def Toy_Dialect : Dialect {
    let name = "toy";
    let cppNamespace = "::toy";
    let summary = "Toy programming language dialect";
    let description = [{ The Toy dialect represents operations and semantics of the Toy programming language. }];
}

class Toy_Op<string mnemonic> : Op<Toy_Dialect, mnemonic>;

