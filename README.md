# toy-compiler 
 toy compiler project 
# Toy Compiler

A small programming language compiler built from scratch in C++ and progressively lowered through **MLIR** to **LLVM IR**.

This project is primarily an educational compiler-engineering project. It explores how a programming language moves from source text through lexical analysis, parsing, an abstract syntax tree, a custom MLIR dialect, MLIR transformation passes, and finally LLVM IR.

The compiler is being developed incrementally, with each stage introducing a real compiler concept and integrating it into the existing pipeline.

---

## What This Project Does

The compiler takes a Toy source program such as:

```toy
fun main() {
    return 42;
}
```

and processes it through several representations:

```text
Toy source
    │
   Lexer     
       │ tokens
   Parser    
       │
     AST     
       │
  Toy MLIR Dialect  
           │
 MLIR Transformation
      Passes        
           │
 Standard MLIR      
 func / arith / ... 
           │
     LLVM IR        
```

For the simple program above, the compiler can currently reach LLVM IR equivalent to:

```llvm
define i32 @main() {
  ret i32 42
}
```

---

# Why This Project Exists

The goal is not simply to create another toy language.

The project is being used to understand compiler construction from the inside:

* How source text becomes tokens
* How tokens become an AST
* How an AST becomes SSA-based IR
* How MLIR dialects are defined
* How operations and regions are represented
* How TableGen generates MLIR boilerplate
* How custom operations are transformed into standard MLIR operations
* How MLIR passes work
* How MLIR eventually becomes LLVM IR

Rather than jumping directly from source code to LLVM IR, this compiler deliberately introduces an intermediate **Toy dialect**.

That makes the project useful for learning modern compiler architecture and MLIR.

---

# Compiler Architecture

The compiler is organized around the following pipeline:

```text
                 Frontend
                    │
              Source Program
                    │
                  Lexer
                    │
                 Tokens
                    │
                 Parser
                    │
                   AST
                    │
              Toy MLIR Dialect
                    │
             MLIR Transformation
                    │
              Standard MLIR
                    │
                LLVM Dialect
                    │
                 LLVM IR
```

Each stage has a specific responsibility.

---

# 1. Lexer

The lexer converts raw source text into tokens.

For example:

```toy
fun main() {
    return 42;
}
```

becomes a sequence similar to:

```text
Identifier : "func"
Identifier : "main"
LParen     : "("
RParen     : ")"
LBrace     : "{"
Return     : "return"
Integer    : "42"
Semicolon  : ";"
RBrace     : "}"
Eof
```

The lexer is implemented in:

```text
include/toy/Lexer.h
lib/Lexer.cpp
```

There is also a dedicated executable:

```text
toy-lex
```

which allows the lexer to be tested independently.

Example:

```bash
./build/toy-lex example/simple.toy
```

---

# 2. Parser

The parser consumes the tokens produced by the lexer and constructs an Abstract Syntax Tree.

The parser is implemented as a recursive descent parser.

Relevant files:

```text
include/toy/Parser.h
lib/Parser.cpp
```

The parser understands constructs such as:

* Function declarations
* Statements
* Return statements
* Integer literals
* Variables
* Binary expressions
* Function calls
* Control flow constructs as they are introduced into the language

The parser therefore transforms:

```text
tokens
   ↓
AST
```

---

# 3. Abstract Syntax Tree

The AST represents the source program in a structured form.

The AST implementation lives in:

```text
include/toy/AST.h
lib/AST.cpp
```

The project contains AST nodes for concepts such as:

```text
Program
FunctionDecl
IntegerLiteral
VariableReference
BinaryExpression
VariableDeclaration
CallExpression
```

The AST separates language syntax from the later MLIR representation.

There is also an AST printer:

```text
include/toy/ASTPrinter.h
lib/ASTPrinter.cpp
```

For example:

```text
Program
  Function: main
    Return
      Integer: 42
```

This makes the frontend easier to debug before MLIR lowering begins.

---

# 4. Toy MLIR Dialect

Once the AST exists, the compiler lowers it into a custom MLIR dialect named:

```text
toy
```

The dialect is defined in:

```text
dialects/ToyDialect.td
```

The dialect currently uses the `toy` namespace:

```text
toy.constant
toy.add
toy.sub
toy.mul
toy.div
toy.return
toy.func
```

The purpose of the custom dialect is to represent the semantics of the Toy language before converting them into more general MLIR dialects.

This gives the compiler an intermediate representation that is specific to the source language.

---

# 5. MLIR TableGen

The Toy dialect uses MLIR TableGen to describe its operations.

The operation definitions are located in:

```text
dialects/operations/ToyOps.td
```

For example, the Toy addition operation is described conceptually as:

```text
toy.add
    operands:
        lhs : i32
        rhs : i32

    result:
        i32
```

Similar operations exist for subtraction, multiplication, and division.

TableGen then generates the C++ declarations and definitions needed by MLIR.

The generated files include:

```text
ToyDialect.h.inc
ToyDialect.cpp.inc
ToyOps.h.inc
ToyOps.cpp.inc
```

These files are generated during the CMake build and should not be edited manually.

---

# 6. Custom Operations

The Toy dialect has gradually evolved from representing simple expressions into representing language level constructs.

Current custom operations include:

### `toy.constant`

Represents an integer constant.

```mlir
%0 = "toy.constant"() <{value = 42 : i32}> : () -> i32
```

### `toy.add`

Represents integer addition.

```mlir
%2 = "toy.add"(%0, %1) : (i32, i32) -> i32
```

### `toy.sub`

Represents subtraction.

### `toy.mul`

Represents multiplication.

### `toy.div`

Represents signed integer division.

### `toy.return`

Represents returning a value from a Toy function.

```mlir
"toy.return"(%0) : (i32) -> ()
```

The operation is marked as an MLIR terminator.

### `toy.fun`

Represents a Toy language function.

It contains:

* A symbol name
* A region
* Blocks containing Toy operations

For example:

```mlir
"toy.fun"() <{sym_name = "main"}> ({
    %0 = "toy.constant"() <{value = 42 : i32}> : () -> i32
    "toy.return"(%0) : (i32) -> ()
}) : () -> ()
```

---

# 7. AST Toy MLIR Lowering

The main AST to MLIR lowering implementation is located in:

```text
include/toy/Lowering.h
lib/Lowering.cpp
```

The lowering process walks the AST and constructs MLIR operations.

For example:

```text
IntegerLiteral(42)
```

becomes:

```text
toy.constant
```

and:

```text
10 + 20
```

becomes conceptually:

```text
toy.constant 10
toy.constant 20
toy.add
```

The lowering stage therefore transforms:

```text
AST
 |
Toy MLIR
```

---

# 8. Symbol Handling

The lowering stage also maintains symbol information while translating expressions.

This becomes important when the language contains variables and later function arguments.

The compiler can therefore move beyond simply compiling isolated constants and expressions toward representing actual programs.

---

# 9. MLIR Transformation Passes

The compiler does not directly translate every Toy operation into LLVM IR.

Instead, it uses MLIR transformation passes.

Current transformation code includes:

```text
lib/ConstantToArith.cpp
lib/ReturnToFunc.cpp
lib/FuncToFunc.cpp
```

with corresponding headers under:

```text
include/toy/
```

These passes demonstrate an important MLIR principle:

```text
Custom dialect
      |
Rewrite / conversion pass
      |
Standard MLIR dialect
```

---

# 10. `toy.constant`  `arith.constant`

The `toy.constant` operation is transformed into the standard MLIR `arith` dialect.

Toy representation:

```mlir
%0 = "toy.constant"() <{value = 42 : i32}> : () -> i32
```

After lowering:

```mlir
%c42_i32 = arith.constant 42 : i32
```

This transformation is implemented in:

```text
lib/ConstantToArith.cpp
```

This is one of the first examples in the project of lowering a language-specific operation into an operation provided by MLIR itself.

---

# 11. `toy.return` -> `func.return`

The Toy return operation is lowered to MLIR's standard function return operation.

Before:

```mlir
"toy.return"(%0) : (i32) -> ()
```

After:

```mlir
return %0 : i32
```

The implementation lives in:

```text
lib/ReturnToFunc.cpp
```

This pass uses MLIR's pattern rewriting infrastructure.

Conceptually:

```text
toy.return
     │
     ▼
func.return
```

---

# 12. `toy.fun` → `func.func`

The Toy dialect also contains a custom function operation:

```text
toy.fun
```

This is lowered into MLIR's standard:

```text
fun.func
```

The transformation is implemented in:

```text
lib/FuncToFunc.cpp
```

The transformation has to preserve the function's:

* symbol name
* region
* entry block
* operations inside the body

This is an important step because MLIR's standard `fun` dialect provides the infrastructure expected by later lowering stages.

---

# 13. The Pass Pipeline

The compiler driver is:

```text
tools/toy-parse.cpp
```

It performs the complete frontend and lowering pipeline.

Conceptually:

```text
Parse source
     |
Construct AST
     |
Lower AST to Toy MLIR
     |
Verify MLIR
     |
Run Toy transformation passes
     |
Canonicalize
     |
Lower toward LLVM
     |
Emit LLVM IR
```

The compiler therefore acts as both a language frontend and an MLIR-based compiler driver.

---

# 14. LLVM IR

The final target of the current pipeline is LLVM IR.

For:

```toy
fun main() {
    return 42;
}
```

the compiler can reach:

```llvm
define i32 @main() {
  ret i32 42
}
```

This is significant because the Toy language is no longer simply being interpreted or printed as an AST.

It has been transformed into a representation understood by LLVM.

The overall path is:

```text
Toy source
    |
Tokens
    |
AST
    |
Toy dialect
    |
Standard MLIR
    |
LLVM dialect
    |
LLVM IR
```

---

# Project Structure

The repository is organized as follows:

```text
toy-compiler-/
│
├── CMakeLists.txt
├── README.md
│
├── dialects/
│   ├── ToyDialect.td
│   |── operations/
│       |── ToyOps.td
│
├── include/
│   |── toy/
│       ├── AST.h
│       ├── ASTPrinter.h
│       ├── Lexer.h
│       ├── Parser.h
│       ├── Lowering.h
│       ├── ToyDialect.h
│       ├── ToyOps.h
│       ├── ConstantToArith.h
│       ├── ReturnToFunc.h
│       |── FuncToFunc.h
│
├── lib/
│   ├── AST.cpp
│   ├── ASTPrinter.cpp
│   ├── Lexer.cpp
│   ├── Parser.cpp
│   ├── Lowering.cpp
│   ├── ToyDialect.cpp
│   ├── ToyOps.cpp
│   ├── ConstantToArith.cpp
│   ├── ReturnToFunc.cpp
│   |── FuncToFunc.cpp
│
├── tools/
│   ├── toy-lex.cpp
│   |── toy-parse.cpp
│
|── example/
    |── simple.toy
```

The current repository structure confirms the separation between the frontend (`Lexer`, `Parser`, AST), MLIR implementation (`Lowering`, dialect and operations), transformation passes, and compiler tools.

---

# Building

The project uses CMake and requires LLVM/MLIR.

The CMake configuration expects LLVM and MLIR to be discoverable through:

```bash
CMAKE_PREFIX_PATH
```

For example:

```bash
cmake .. \
  -DCMAKE_PREFIX_PATH=$HOME/llvm-project-22.1.0.src/build-rtti
```

Then build:

```bash
make -j1
```

The build produces the compiler tools:

```text
toy-lex
toy-parse
```

---

# Running the Lexer

From the project root:

```bash
./build/toy-lex example/simple.toy
```

This prints the token stream produced by the lexer.

---

# Running the Compiler

The main compiler driver is:

```bash
./build/toy-parse example/simple.toy
```

It prints the AST and the intermediate representations produced during compilation.

For example:

```text
Program
  Function: main
    Return
      Integer: 42
```

followed by the Toy MLIR representation and later lowered MLIR/LLVM IR.

---

# Example

Input:

```toy
func main() {
    return 42;
}
```

AST:

```text
Program
  Function: main
    Return
      Integer: 42
```

Toy dialect:

```mlir
module {
  "toy.func"() <{sym_name = "main"}> ({
    %0 = "toy.constant"() <{value = 42 : i32}> : () -> i32
    "toy.return"(%0) : (i32) -> ()
  }) : () -> ()
}
```

Standard MLIR:

```mlir
module {
  func.func @main() -> i32 {
    %c42_i32 = arith.constant 42 : i32
    return %c42_i32 : i32
  }
}
```

LLVM IR:

```llvm
define i32 @main() {
  ret i32 42
}
```

This example demonstrates the central idea of the entire project: **one program represented at multiple compiler stages.**

---

# Development Roadmap

The compiler is being developed incrementally.

The current custom-dialect roadmap includes:

* |x| `toy.constant`
* |x| `toy.add`
* |x| `toy.sub`
* |x| `toy.mul`
* |x| `toy.div`
* |x| `toy.return`
* |x| `toy.func`
* | | `toy.call`
* | | Function arguments
* | | `toy.print`
* | | Operation verification
* | | Operation builders
* | | Operation traits
* | | Regions and blocks in custom operations
* | | Completing the Toy dialect
* | | Full Toy  Standard MLIR lowering

The project is deliberately being developed one compiler concept at a time.

---

# Learning Goals

This project explores practical implementations of:

### Frontend

* Lexical analysis
* Tokenization
* Recursive descent parsing
* Abstract syntax trees
* Expression parsing
* Operator precedence
* Function declarations
* Function calls

### MLIR

* MLIRContext
* ModuleOp
* Operations
* Values
* Types
* Attributes
* Regions
* Blocks
* SSA values
* Dialects
* TableGen
* Operation definitions
* Operation traits
* Operation verification
* Pattern rewriting
* RewritePattern
* PatternRewriter
* PassWrapper
* PassManager
* Canonicalization
* Dialect lowering

### Backend

* Standard MLIR dialects
* LLVM dialect
* LLVM IR generation
* Lowering toward executable code

---

# Why Use MLIR?

A traditional educational compiler might translate directly:

```text
AST - LLVM IR
```

This project instead uses:

```text
AST
 |
Toy Dialect
 |
Standard MLIR
 |
LLVM
```

The extra intermediate representation makes the compiler more modular.

The Toy dialect captures concepts that belong specifically to the Toy language.

Standard MLIR dialects then provide reusable infrastructure for concepts that are common across many languages.

For example:

```text
toy.constant
      |
arith.constant
```

and:

```text
toy.return
      |
func.return
```

This separation makes it possible to study how real world compiler infrastructures such as MLIR handle dialects, transformations, and progressive lowering.

---

# Development Philosophy

The project is intentionally built incrementally.

Instead of implementing the entire compiler at once, each feature follows a path similar to:

```text
Language feature
      |
AST representation
      |
Toy dialect operation
      |
TableGen definition
      |
MLIR construction
      |
Transformation pass
      |
Standard MLIR
      |
LLVM
```

This makes each compiler feature independently understandable and testable.

It also makes the commit history an important part of the project's development story: the compiler evolves from a small frontend into a progressively more complete MLIR based compiler.

---

# Current Status

The compiler currently has a functioning frontend and an MLIR based backend pipeline.

The following major components are already in place:

* Hand written lexer
* Recursive descent parser
* AST representation
* AST printer
* AST MLIR lowering
* Custom `toy` dialect
* TableGen generated operations
* Integer arithmetic operations
* Custom return operation
* Custom function operation
* MLIR rewrite passes
* Toy  standard MLIR transformations
* LLVM dialect translation
* LLVM IR generation

The compiler is still under active development.

The next major feature is **`toy.call`**, followed by function arguments and the remaining custom dialect infrastructure.

---

# Project Status at a Glance

```text
Frontend : Working

AST : Working

Toy Dialect : In progress

MLIR Lowering : In progress

LLVM Lowering : In progress

Complete Toy Language : In progress
```

---

# Technologies

The project currently uses:

* **C++17**
* **CMake**
* **LLVM**
* **MLIR**
* **MLIR TableGen**
* **Ninja / Make**
* **LLVM IR**

The repository's CMake configuration explicitly enables C and C++, requires C++17, discovers LLVM and MLIR, and uses MLIR/LLVM CMake modules and TableGen generation.

---

# What This Project Demonstrates

At its core, this repository demonstrates how to build a compiler rather than simply how to write a parser.

The important transformation is:

```text
"return 42"
```

is not treated as one giant compiler operation.

Instead, it becomes a sequence of progressively lower level representations:

```text
Source
  |
Tokens
  |
AST
  |
Toy operations
  |
Standard MLIR operations
  |
LLVM dialect
  |
LLVM IR
```

Each stage removes language specific information while preserving the program's meaning.

That is the central idea behind this compiler.

---

# Future Work

The immediate development goals are:

1. Implement `toy.call`
2. Represent function arguments
3. Add `toy.print`
4. Add operation verification
5. Improve operation builders
6. Explore operation traits
7. Expand region/block support
8. Complete the Toy dialect
9. Finish the Toy  Standard MLIR lowering pipeline
10. Continue toward a more complete compiler backend

---

# License

No license is currently specified in the repository. If this project is intended for public reuse, a license should be added explicitly.

# About ME 
email: ivomofor@live.com
Cell: +27670771540
I am a motivated CS graduate with a strong foundation in compiler theory, seeking an entry level compiler engineering role. Equipped with hands on academic experience in C++, Clang, MLIR and LLVM. I aim to deepen my expertise in IR design and optimization passes while contributing meaningfully to real world compiler infrastructure.

