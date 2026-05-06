/*
 * semantic.h - Semantic Analysis interface
 *
 * Walks the AST and validates:
 *   - Variables are declared before use
 *   - Expressions are valid
 * Reports errors with line information.
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol_table.h"

/* -------------------------------------------------------
   Semantic Analyser Context
   ------------------------------------------------------- */
typedef struct {
    SymbolTable *table;
    int          error_count;
} SemanticCtx;

/* -------------------------------------------------------
   API
   ------------------------------------------------------- */

/* Create a new semantic analysis context */
SemanticCtx *semantic_create(void);

/* Free the context */
void semantic_destroy(SemanticCtx *ctx);

/* Run semantic analysis on the whole program.
   Returns 0 on success, non-zero if errors found. */
int semantic_analyse(SemanticCtx *ctx, ASTNode *root);

#endif /* SEMANTIC_H */
