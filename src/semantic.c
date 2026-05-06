/*
 * semantic.c - Semantic Analysis implementation
 *
 * Two-pass approach over the AST:
 *   Pass 1 (collect): register all assignment targets.
 *   Pass 2 (check):   verify all variable references exist.
 *
 * This simple strategy avoids false "undeclared" errors for
 * forward-declared variables in the same block.
 */

#include <stdio.h>
#include <stdlib.h>
#include "semantic.h"
#include "ast.h"
#include "symbol_table.h"

/* -------------------------------------------------------
   semantic_create
   ------------------------------------------------------- */
SemanticCtx *semantic_create(void) {
    SemanticCtx *ctx = (SemanticCtx *)malloc(sizeof(SemanticCtx));
    if (!ctx) { fprintf(stderr, "[Semantic] Out of memory\n"); exit(1); }
    ctx->table       = sym_table_create();
    ctx->error_count = 0;
    return ctx;
}

/* -------------------------------------------------------
   semantic_destroy
   ------------------------------------------------------- */
void semantic_destroy(SemanticCtx *ctx) {
    sym_table_destroy(ctx->table);
    free(ctx);
}

/* -------------------------------------------------------
   Pass 1: collect all assignment targets into symbol table
   ------------------------------------------------------- */
static void collect_declarations(SemanticCtx *ctx, ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            for (int i = 0; i < node->program.count; i++)
                collect_declarations(ctx, node->program.stmts[i]);
            break;

        case NODE_ASSIGN:
            /* Register the variable being assigned */
            sym_table_insert(ctx->table, node->assign.var_name);
            break;

        default:
            break;
    }
}

/* -------------------------------------------------------
   Pass 2: check all variable references
   ------------------------------------------------------- */
static void check_node(SemanticCtx *ctx, ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            for (int i = 0; i < node->program.count; i++)
                check_node(ctx, node->program.stmts[i]);
            break;

        case NODE_ASSIGN:
            /* Check the expression on the right-hand side */
            check_node(ctx, node->assign.expr);
            break;

        case NODE_PRINT:
            check_node(ctx, node->print.expr);
            break;

        case NODE_BINOP:
            check_node(ctx, node->binop.left);
            check_node(ctx, node->binop.right);
            break;

        case NODE_VAR:
            /* Variable must have been declared */
            if (!sym_table_lookup(ctx->table, node->name)) {
                fprintf(stderr,
                    "[Semantic Error] Undeclared variable: '%s'\n",
                    node->name);
                ctx->error_count++;
            }
            break;

        case NODE_NUMBER:
            /* Always valid */
            break;
    }
}

/* -------------------------------------------------------
   semantic_analyse: run both passes
   ------------------------------------------------------- */
int semantic_analyse(SemanticCtx *ctx, ASTNode *root) {
    printf("[Semantic] Starting analysis...\n");

    /* Pass 1: gather all declared variables */
    collect_declarations(ctx, root);

    /* Pass 2: validate all references */
    check_node(ctx, root);

    if (ctx->error_count == 0) {
        printf("[Semantic] Analysis passed with no errors.\n");
        /* Optionally dump the symbol table */
        sym_table_dump(ctx->table);
        return 0;
    }

    fprintf(stderr, "[Semantic] %d error(s) found.\n", ctx->error_count);
    return ctx->error_count;
}
