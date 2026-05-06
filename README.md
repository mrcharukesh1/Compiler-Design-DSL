# Compiler-Design-DSL

A complete end-to-end compiler for a custom Domain-Specific Language (DSL), built using **Flex**, **Bison**, **C**, and **LLVM IR** generation. This project demonstrates all major phases of a real compiler pipeline.

---

## Features

- **Lexical Analysis** — Flex tokenizer recognising keywords, identifiers, integers, and operators
- **Parsing** — Bison grammar with correct operator precedence, building a full AST
- **AST Construction** — Modular C structs and enums for every language construct
- **Symbol Table** — Hash-table implementation for variable tracking
- **Semantic Analysis** — Two-pass analysis detecting undeclared variables
- **LLVM IR Generation** — Manual `fprintf`-based emission of valid `.ll` files (no LLVM API)
- **Native Execution** — Full pipeline to a runnable binary via `llc` + `clang`

---

## DSL Syntax

```
begin

  x = 10;
  y = 20;

  print x + y;
  print x * y;

end
```

### Supported constructs
| Construct    | Example          |
|--------------|------------------|
| Assignment   | `x = 42;`        |
| Print        | `print x + 1;`   |
| Add          | `x + y`          |
| Subtract     | `x - y`          |
| Multiply     | `x * y`          |
| Divide       | `x / y`          |
| Parentheses  | `(x + y) * 2`    |

---

## Folder Structure

```
Compiler-Design-DSL/
├── README.md
├── report.pdf
├── .gitignore
├── src/
│   ├── lexer.l          # Flex lexer
│   ├── parser.y         # Bison grammar
│   ├── ast.h / ast.c    # AST node types and constructors
│   ├── semantic.h / .c  # Semantic analysis
│   ├── symbol_table.h/.c# Hash-table symbol table
│   ├── codegen.h / .c   # LLVM IR generation
│   ├── main.c           # Compiler driver
│   └── Makefile
├── docs/
│   ├── architecture.md
│   └── grammar.md
├── test/
│   ├── sample1.dsl
│   ├── sample2.dsl
│   └── sample_error.dsl
└── output/
    ├── output.ll
    ├── output.s
    └── executable
```

---

## Technologies Used

| Tool      | Purpose                            |
|-----------|------------------------------------|
| Flex      | Lexical analysis (tokenisation)    |
| Bison     | LALR(1) parser + AST construction  |
| C (GCC)   | All compiler logic                 |
| LLVM IR   | Target intermediate representation |
| llc       | IR → assembly                      |
| clang     | Assembly → native executable       |

---

## How to Run

### Prerequisites

```bash
sudo apt install flex bison gcc llvm clang   # Debian/Ubuntu
brew install flex bison llvm                 # macOS
```

### Build the compiler

```bash
cd src/
make
```

### Compile a DSL program

```bash
./compiler ../test/sample1.dsl ../output/output.ll
```

### Generate and run native executable

```bash
make ir
# or manually:
llc ../output/output.ll -o ../output/output.s
clang ../output/output.s -o ../output/executable
../output/executable
```

### Run all tests

```bash
make test
```

---

## Compilation Pipeline

```
source.dsl
    │
    ▼
[Flex Lexer]   →  Token stream
    │
    ▼
[Bison Parser] →  Abstract Syntax Tree (AST)
    │
    ▼
[Semantic Analyser] + [Symbol Table]
    │
    ▼
[LLVM IR Codegen]  →  output.ll
    │
    ▼
[llc]  →  output.s  (assembly)
    │
    ▼
[clang] →  executable
    │
    ▼
Native binary output
```

---

## Sample Input / Output

**Input (`sample1.dsl`):**
```
begin
  x = 10;
  y = 20;
  print x + y;
  print x * y;
end
```

**Compiler output:**
```
[Phase 1/2] Parsing completed successfully.
[Phase 3]   Semantic analysis passed.
[Phase 4]   IR written to: ../output/output.ll
```

**Executable output:**
```
30
200
```

---

## Report

See `report.pdf` for the full project report covering architecture, grammar, AST design, semantic analysis, IR generation, and results.

---

## Team Details

| Field        | Details                         |
|--------------|---------------------------------|
| Project Name | Compiler Design DSL             |
| Subject      | Compiler Design                 |
| Technology   | Flex, Bison, C, LLVM IR         |

---

## License

MIT License — free to use for academic and personal projects.
