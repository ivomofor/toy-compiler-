/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Dialect Declarations                                                       *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|* From: ToyDialect.td                                                        *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

namespace toy {

/// Toy programming language dialect
/// The Toy dialect represents operations and semantics of the Toy programming language.
class ToyDialect : public ::mlir::Dialect {
  explicit ToyDialect(::mlir::MLIRContext *context);

  void initialize();
  friend class ::mlir::MLIRContext;
public:
  ~ToyDialect() override;
  static constexpr ::llvm::StringLiteral getDialectNamespace() {
    return ::llvm::StringLiteral("toy");
  }
};

} // namespace toy
MLIR_DECLARE_EXPLICIT_TYPE_ID(::toy::ToyDialect)
