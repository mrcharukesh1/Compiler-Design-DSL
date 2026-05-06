/*
 * codegen.h - LLVM IR Code Generation interface
 *
 * Generates LLVM IR text (manually via fprintf) from the AST.
 * Does NOT use the LLVM C API — output is a plain .ll file.
 *
 * IR structure emitted:
 *   - declare external printf
 *   - format string global for printing integers
 *   - @main function with alloca / store / load / arithmetic
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include "ast.h"
#include "symbol_table.h"

/* -------------------------------------------------------
   Code Generation Context
   ------------------------------------------------------- */
typedef struct {
    FILE        *out;           /* Output .ll file            */
    SymbolTable *sym;           /* Symbol table from semantic */
    int          tmp_counter;   /* SSA temp register counter  */
} CodegenCtx;

/* -------------------------------------------------------
   API
   ------------------------------------------------------- */

/* Create a codegen context writing to `out_path` */
CodegenCtx *codegen_create(const char *out_path, SymbolTable *sym);

/* Free resources */
void codegen_destroy(CodegenCtx *ctx);

/* Generate full LLVM IR from the AST root */
void codegen_generate(CodegenCtx *ctx, ASTNode *root);

#endif /* CODEGEN_H */
