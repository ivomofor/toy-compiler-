/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Interface Declarations                                                     *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

namespace toy {

class ExampleInterface;

} // namespace toy
namespace toy {

namespace detail {
struct ExampleInterfaceInterfaceTraits {
  struct Concept {
    /// The methods defined by the interface.
    llvm::StringRef (*getToyName)(const Concept *impl, ::mlir::Operation *);
  };
  template<typename ConcreteOp>
  class Model : public Concept {
  public:
    using Interface = ::toy::ExampleInterface;
    Model() : Concept{getToyName} {}

    static inline llvm::StringRef getToyName(const Concept *impl, ::mlir::Operation *tablegen_opaque_val);
  };
  template<typename ConcreteOp>
  class FallbackModel : public Concept {
  public:
    using Interface = ::toy::ExampleInterface;
    FallbackModel() : Concept{getToyName} {}

    static inline llvm::StringRef getToyName(const Concept *impl, ::mlir::Operation *tablegen_opaque_val);
  };
  template<typename ConcreteModel, typename ConcreteOp>
  class ExternalModel : public FallbackModel<ConcreteModel> {
  public:
    using ConcreteEntity = ConcreteOp;
  };
};
template <typename ConcreteOp>
struct ExampleInterfaceTrait;

} // namespace detail
class ExampleInterface : public ::mlir::OpInterface<ExampleInterface, detail::ExampleInterfaceInterfaceTraits> {
public:
  using ::mlir::OpInterface<ExampleInterface, detail::ExampleInterfaceInterfaceTraits>::OpInterface;
  template <typename ConcreteOp>
  struct Trait : public detail::ExampleInterfaceTrait<ConcreteOp> {};

  /// Get the Toy name of the operation
  llvm::StringRef getToyName();
};

} // namespace toy
namespace toy::detail {

  template <typename ConcreteOp>
  struct ExampleInterfaceTrait : public ::mlir::OpInterface<ExampleInterface, detail::ExampleInterfaceInterfaceTraits>::Trait<ConcreteOp> {
  };

} // namespace toy::detail
namespace toy {

template<typename ConcreteOp>
llvm::StringRef detail::ExampleInterfaceInterfaceTraits::Model<ConcreteOp>::getToyName(const Concept *impl, ::mlir::Operation *tablegen_opaque_val) {
  return (llvm::cast<ConcreteOp>(tablegen_opaque_val)).getToyName();
}
template<typename ConcreteOp>
llvm::StringRef detail::ExampleInterfaceInterfaceTraits::FallbackModel<ConcreteOp>::getToyName(const Concept *impl, ::mlir::Operation *tablegen_opaque_val) {
  return static_cast<const ConcreteOp *>(impl)->getToyName(tablegen_opaque_val);
}

} // namespace toy
