/*
 * ast.c - AST node construction and utilities
 *
 * Implements constructors for every AST node type,
 * a recursive printer for debugging, and a tree-free function.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* -------------------------------------------------------
   Internal helper: allocate a zeroed AST node
   ------------------------------------------------------- */
static ASTNode *new_node(NodeType type) {
    ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "[AST] Out of memory\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    return node;
}

/* -------------------------------------------------------
   make_number: create an integer literal node
   ------------------------------------------------------- */
ASTNode *make_number(int value) {
    ASTNode *n = new_node(NODE_NUMBER);
    n->ival    = value;
    return n;
}

/* -------------------------------------------------------
   make_var: create a variable reference node
   ------------------------------------------------------- */
ASTNode *make_var(const char *name) {
    ASTNode *n = new_node(NODE_VAR);
    n->name    = strdup(name);
    return n;
}

/* -------------------------------------------------------
   make_binop: create a binary operation node
   ------------------------------------------------------- */
ASTNode *make_binop(BinOpType op, ASTNode *left, ASTNode *right) {
    ASTNode *n     = new_node(NODE_BINOP);
    n->binop.op    = op;
    n->binop.left  = left;
    n->binop.right = right;
    return n;
}

/* -------------------------------------------------------
   make_assign: create an assignment statement node
   ------------------------------------------------------- */
ASTNode *make_assign(const char *var_name, ASTNode *expr) {
    ASTNode *n        = new_node(NODE_ASSIGN);
    n->assign.var_name = strdup(var_name);
    n->assign.expr    = expr;
    return n;
}

/* -------------------------------------------------------
   make_print: create a print statement node
   ------------------------------------------------------- */
ASTNode *make_print(ASTNode *expr) {
    ASTNode *n    = new_node(NODE_PRINT);
    n->print.expr = expr;
    return n;
}

/* -------------------------------------------------------
   make_program: create the root program node
   ------------------------------------------------------- */
ASTNode *make_program(ASTNode **stmts, int count) {
    ASTNode *n       = new_node(NODE_PROGRAM);
    n->program.stmts = stmts;
    n->program.count = count;
    return n;
}

/* -------------------------------------------------------
   free_ast: recursively free AST memory
   ------------------------------------------------------- */
void free_ast(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_NUMBER:
            break;
        case NODE_VAR:
            free(node->name);
            break;
        case NODE_BINOP:
            free_ast(node->binop.left);
            free_ast(node->binop.right);
            break;
        case NODE_ASSIGN:
            free(node->assign.var_name);
            free_ast(node->assign.expr);
            break;
        case NODE_PRINT:
            free_ast(node->print.expr);
            break;
        case NODE_PROGRAM:
            for (int i = 0; i < node->program.count; i++)
                free_ast(node->program.stmts[i]);
            free(node->program.stmts);
            break;
    }
    free(node);
}

/* -------------------------------------------------------
   print_ast: debug printer (indented tree view)
   ------------------------------------------------------- */
static const char *op_str(BinOpType op) {
    switch (op) {
        case OP_ADD: return "+";
        case OP_SUB: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
    }
    return "?";
}

void print_ast(ASTNode *node, int indent) {
    if (!node) return;

    for (int i = 0; i < indent; i++) printf("  ");

    switch (node->type) {
        case NODE_PROGRAM:
            printf("[Program] (%d statements)\n", node->program.count);
            for (int i = 0; i < node->program.count; i++)
                print_ast(node->program.stmts[i], indent + 1);
            break;

        case NODE_ASSIGN:
            printf("[Assign] %s =\n", node->assign.var_name);
            print_ast(node->assign.expr, indent + 1);
            break;

        case NODE_PRINT:
            printf("[Print]\n");
            print_ast(node->print.expr, indent + 1);
            break;

        case NODE_BINOP:
            printf("[BinOp] %s\n", op_str(node->binop.op));
            print_ast(node->binop.left,  indent + 1);
            print_ast(node->binop.right, indent + 1);
            break;

        case NODE_NUMBER:
            printf("[Number] %d\n", node->ival);
            break;

        case NODE_VAR:
            printf("[Var] %s\n", node->name);
            break;
    }
}
