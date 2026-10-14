# Toy Compiler — Building a Compiler with MLIR

A small programming language compiler written in **C++17** to learn compiler construction through hands on work with **LLVM and MLIR**.

This project follows a Toy program from source code through lexical analysis, parsing, an Abstract Syntax Tree (AST), a custom MLIR dialect, transformation passes, and lower level intermediate representations. It is an educational project for understanding how compiler frontends, MLIR dialects, rewrite patterns, and lowering pipelines fit together.

## Compiler pipeline

```text
Toy source
   |
 Lexer -> Tokens
   |
 Parser -> AST
   |
 AST-to-MLIR lowering
   |
 Custom Toy dialect
   |
 Toy transformation passes
   |
 Standard MLIR dialects
   |
 LLVM dialect / LLVM IR
   |
 Object code or executable
```

The compiler is developed incrementally; not every language feature is supported end to end yet.

## Example Toy program

```toy
fun main() {
    return 42;
}
```

The frontend tokenizes and parses the program, builds an AST, and lowers it to the custom Toy dialect. Transformation passes demonstrate how Toy specific operations can be replaced with standard MLIR operations. A simple return-value program can ultimately be represented in LLVM IR in a form similar to:

```llvm
define i32 @main() {
  ret i32 42
}
```

## Implemented areas

### Frontend
- Hand written lexer and recursive descent parser.
- AST nodes and an AST printer.
- Integer literals and arithmetic expressions with operator precedence.
- Variable declarations and references.
- Function declarations and parameters.
- Return statements and function call parsing.
- Conditional and loop constructs as they are introduced.
- A standalone `toy-lex` tool for inspecting tokens.

### Custom Toy dialect
The custom `toy` dialect is described using MLIR TableGen. Operations include:

- `toy.constant` — integer constants.
- `toy.add`, `toy.sub`, `toy.mul`, and `toy.div` — integer arithmetic.
- `toy.return` — return a value.
- `toy.fun` — a Toy function with a body region and parameters.
- `toy.print` — print a Toy integer.
- `toy.tensor.matmul` — tensor matrix multiplication.

The project also explores TableGen generated operations, custom builders, verification, operation interfaces, regions, blocks, and SSA values.

### MLIR transformations
Passes demonstrate lowering custom operations to standard MLIR operations:

- `toy.constant` to `arith.constant`.
- `toy.return` to `func.return`.
- `toy.fun` to `func.func`.
- `toy.print` to a call to the Toy runtime print function.
- `toy.tensor.matmul` to Linalg matrix multiplication operations.

The rewrite work uses concepts including `OpRewritePattern`, `PatternRewriter`, `RewritePatternSet`, and greedy pattern application.

### Tensor and Linalg
The tensor work explores tensor value semantics versus memory-oriented `memref` values and lowers the custom matrix multiplication operation to structured Linalg operations. The current lowering creates an output tensor with `tensor.empty`, initializes it to zero with `linalg.fill`, and passes it to `linalg.matmul`. It supports tested static and dynamic output dimensions and includes tests for valid and invalid shapes and types.

These tests validate IR construction and lowering; they do not, by themselves, demonstrate runtime numerical execution of the generated matrix multiplication.

## Technology stack

- C++17
- LLVM and MLIR
- MLIR TableGen
- CMake
- Ninja or Make

The project requires an LLVM/MLIR development installation compatible with the APIs used by the code.

## Repository layout

```text
toy-compiler-/
── CMakeLists.txt
── dialects/
   ── ToyDialect.td
   ── operations/ToyOps.td
── include/toy/       # Frontend, dialect, and pass headers
── lib/               # Frontend, dialect, and lowering implementations
── runtime/
   ── toy_runtime.cpp
── tools/
   ── toy-lex.cpp
   ── toy-parse.cpp
   ── toy-verify.cpp
   ── toy-matmul-test.cpp
   ── toy-matmul-invalid-test.cpp
   ── toy-matmul-valid-test.cpp
── example/
    ── simple.toy
```

This is a simplified overview; some headers and supporting files are omitted. TableGen generated files are created during the build and should not be edited manually.

## Build

You need CMake, a C++17 compiler, and an LLVM/MLIR installation with development files. From the repository root, configure CMake using your LLVM/MLIR installation prefix. For the setup used during development:

```bash
cmake -S . -B build \
  -DCMAKE_PREFIX_PATH="$HOME/LLVM-22.1.0-Linux-X64"
```

If your installation is elsewhere, replace the prefix path. Build with one job to reduce memory use:

```bash
cmake --build build -j1
```

## Run the tools

```bash
./build/toy-lex example/simple.toy
./build/toy-parse example/simple.toy
```

The lexer prints tokens; the compiler driver prints intermediate representations produced by the frontend and lowering pipeline. The repository also defines verification and matrix-multiplication test executables:

```bash
./build/toy-verify
./build/toy-matmul-test
./build/toy-matmul-invalid-test
./build/toy-matmul-valid-test
```

Some tools may expect particular arguments or test inputs; consult the corresponding source file if a tool does not run without arguments.

## Learning topics

- Lexing, parsing, ASTs, and operator precedence.
- MLIR contexts, modules, operations, types, attributes, regions, blocks, and SSA values.
- Custom dialects and TableGen.
- Operation builders, verification, interfaces, and traits.
- Rewrite patterns, passes, and canonicalization.
- Lowering Toy operations to standard MLIR.
- Tensor and MemRef concepts.
- Structured Linalg operations and matrix multiplication.
- LLVM dialect and LLVM IR generation.

## Project status

The frontend, custom Toy dialect, several Toy-to-standard-MLIR transformations, and tensor matrix-multiplication lowering are implemented as part of this learning project. The compiler is still evolving, and some language features and end-to-end lowering paths remain works in progress.

The aim is to understand compiler engineering by implementing and testing each stage rather than treating compilation as a black box.

## References

- [LLVM](https://llvm.org/)
- [MLIR documentation](https://mlir.llvm.org/)
- [MLIR Toy tutorial](https://mlir.llvm.org/docs/Tutorials/Toy/)

## About

I am a Computer Science graduate building practical experience in compiler engineering, C++, LLVM, and MLIR. This repository documents my hands on learning in intermediate representation design, dialect development, and compiler lowering.

## License

Check the repository for a `LICENSE` file. If no license is present, reuse permissions have not been explicitly specified.
