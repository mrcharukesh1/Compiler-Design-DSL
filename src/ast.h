/*
 * ast.h - Abstract Syntax Tree definitions
 *
 * Defines node types and structures for the AST.
 * Each node represents a construct in the DSL grammar.
 */

#ifndef AST_H
#define AST_H

/* -------------------------------------------------------
   AST Node Types
   ------------------------------------------------------- */
typedef enum {
    NODE_PROGRAM,       /* Root node: list of statements          */
    NODE_ASSIGN,        /* Assignment: identifier = expression    */
    NODE_PRINT,         /* Print statement: print expression      */
    NODE_BINOP,         /* Binary operation: expr op expr         */
    NODE_NUMBER,        /* Integer literal                        */
    NODE_VAR            /* Variable reference                     */
} NodeType;

/* Binary operator types */
typedef enum {
    OP_ADD,  /* + */
    OP_SUB,  /* - */
    OP_MUL,  /* * */
    OP_DIV   /* / */
} BinOpType;

/* -------------------------------------------------------
   AST Node Structure
   ------------------------------------------------------- */
typedef struct ASTNode {
    NodeType type;

    union {
        /* NODE_NUMBER: integer value */
        int ival;

        /* NODE_VAR / NODE_ASSIGN: variable name */
        char *name;

        /* NODE_BINOP: left, right operands and operator */
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            BinOpType       op;
        } binop;

        /* NODE_ASSIGN: variable name + expression */
        struct {
            char           *var_name;
            struct ASTNode *expr;
        } assign;

        /* NODE_PRINT: expression to print */
        struct {
            struct ASTNode *expr;
        } print;

        /* NODE_PROGRAM: list of statement nodes */
        struct {
            struct ASTNode **stmts;
            int              count;
        } program;
    };
} ASTNode;

/* -------------------------------------------------------
   Constructor Functions
   ------------------------------------------------------- */
ASTNode *make_number(int value);
ASTNode *make_var(const char *name);
ASTNode *make_binop(BinOpType op, ASTNode *left, ASTNode *right);
ASTNode *make_assign(const char *var_name, ASTNode *expr);
ASTNode *make_print(ASTNode *expr);
ASTNode *make_program(ASTNode **stmts, int count);

/* Utility: free entire AST */
void free_ast(ASTNode *node);

/* Utility: pretty-print AST (for debugging) */
void print_ast(ASTNode *node, int indent);

#endif /* AST_H */
