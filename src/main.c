/*
 * main.c - Compiler Driver
 *
 * Orchestrates all compiler phases:
 *   1. Open source file and feed to lexer/parser
 *   2. Print the AST (debug)
 *   3. Semantic analysis
 *   4. LLVM IR code generation
 *
 * Usage:
 *   ./compiler <source.dsl>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"
#include "codegen.h"

/* -------------------------------------------------------
   Bison / Flex externals
   ------------------------------------------------------- */
extern FILE    *yyin;           /* Flex input stream          */
extern int      yyparse(void);  /* Bison entry point          */
extern ASTNode *parse_result;   /* Root AST node set by Bison */

/* Default output path for LLVM IR */
#define DEFAULT_IR_OUTPUT "../output/output.ll"

/* -------------------------------------------------------
   Banner
   ------------------------------------------------------- */
static void print_banner(void) {
    printf("====================================================\n");
    printf("  DSL Compiler  –  Compiler Design Project\n");
    printf("  Phases: Lex → Parse → Semantic → Codegen\n");
    printf("====================================================\n\n");
}

/* -------------------------------------------------------
   main
   ------------------------------------------------------- */
int main(int argc, char *argv[]) {
    print_banner();

    /* ---- 0. Argument check ---- */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source.dsl> [output.ll]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *src_path = argv[1];
    const char *ir_path  = (argc >= 3) ? argv[2] : DEFAULT_IR_OUTPUT;

    /* ---- 1. Open source file ---- */
    yyin = fopen(src_path, "r");
    if (!yyin) {
        fprintf(stderr, "[Driver] Cannot open source file: %s\n", src_path);
        return EXIT_FAILURE;
    }
    printf("[Driver] Compiling: %s\n\n", src_path);

    /* ---- 2. Lex + Parse ---- */
    printf("[Phase 1/2] Lexical analysis + Parsing...\n");
    int parse_status = yyparse();
    fclose(yyin);

    if (parse_status != 0 || !parse_result) {
        fprintf(stderr, "[Driver] Parsing failed. Aborting.\n");
        return EXIT_FAILURE;
    }
    printf("[Phase 1/2] Parsing completed successfully.\n\n");

    /* ---- 3. Print AST (debug) ---- */
    printf("[AST]\n");
    print_ast(parse_result, 0);
    printf("\n");

    /* ---- 4. Semantic Analysis ---- */
    printf("[Phase 3] Semantic Analysis...\n");
    SemanticCtx *sem = semantic_create();
    int sem_errors   = semantic_analyse(sem, parse_result);

    if (sem_errors != 0) {
        fprintf(stderr, "[Driver] Semantic errors found. Aborting.\n");
        semantic_destroy(sem);
        free_ast(parse_result);
        return EXIT_FAILURE;
    }
    printf("[Phase 3] Semantic analysis passed.\n\n");

    /* ---- 5. Code Generation ---- */
    printf("[Phase 4] LLVM IR Code Generation...\n");
    CodegenCtx *cg = codegen_create(ir_path, sem->table);
    codegen_generate(cg, parse_result);
    codegen_destroy(cg);
    printf("[Phase 4] IR written to: %s\n\n", ir_path);

    /* ---- 6. Cleanup ---- */
    semantic_destroy(sem);
    free_ast(parse_result);

    printf("====================================================\n");
    printf("  Compilation successful!\n");
    printf("\n  Next steps:\n");
    printf("  llc %s -o ../output/output.s\n", ir_path);
    printf("  clang ../output/output.s -o ../output/executable\n");
    printf("  ../output/executable\n");
    printf("====================================================\n");

    return EXIT_SUCCESS;
}
