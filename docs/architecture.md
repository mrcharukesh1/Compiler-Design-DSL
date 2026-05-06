# Architecture

## Overview

The DSL compiler is structured as a classic multi-phase pipeline. Each phase is implemented in its own module (`.h` + `.c` pair), making the code easy to navigate and extend.

```
┌──────────────┐
│  Source File │  (.dsl)
└──────┬───────┘
       │
       ▼
┌──────────────┐
│  Flex Lexer  │  lexer.l
│  (Tokeniser) │
└──────┬───────┘
       │  Token stream
       ▼
┌──────────────┐
│Bison Parser  │  parser.y
│  + AST Build │
└──────┬───────┘
       │  ASTNode* (tree)
       ▼
┌──────────────────────────┐
│  Semantic Analyser       │  semantic.c
│  + Symbol Table          │  symbol_table.c
└──────┬───────────────────┘
       │  Validated AST
       ▼
┌──────────────┐
│  IR Codegen  │  codegen.c
│  (LLVM .ll)  │
└──────┬───────┘
       │  output.ll
       ▼
┌──────────────┐
│    llc       │  → output.s
└──────┬───────┘
       ▼
┌──────────────┐
│   clang      │  → executable
└──────────────┘
```

## Module Responsibilities

| Module           | File(s)               | Responsibility                        |
|------------------|-----------------------|---------------------------------------|
| Lexer            | `lexer.l`             | Tokenise source text                  |
| Parser           | `parser.y`            | Grammar rules, AST construction       |
| AST              | `ast.h`, `ast.c`      | Node types, constructors, free, print |
| Symbol Table     | `symbol_table.h/.c`   | Variable storage (hash table)         |
| Semantic         | `semantic.h/.c`       | Declaration/use validation            |
| Code Generator   | `codegen.h/.c`        | Emit LLVM IR text                     |
| Driver           | `main.c`              | Phase orchestration                   |

## Data Flow

1. `yyin` is pointed at the source file.
2. `yyparse()` drives the lexer internally; Bison actions call `make_*` constructors to build AST nodes; the root is stored in `parse_result`.
3. `semantic_analyse()` walks the tree twice: once to collect declarations, once to verify uses.
4. `codegen_generate()` walks the validated tree and emits LLVM IR via `fprintf`.
