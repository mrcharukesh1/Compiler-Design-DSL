%{
/*
 * parser.y - Bison Grammar for DSL Compiler
 *
 * Builds an AST from the token stream produced by Flex.
 * Grammar covers: program, statement list, assignment, print,
 * and arithmetic expressions with correct operator precedence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* Root of the AST; set when parsing succeeds */
ASTNode *parse_result = NULL;

/* Statement accumulator during parsing */
#define MAX_STMTS 256
static ASTNode *stmt_buf[MAX_STMTS];
static int      stmt_count = 0;

/* Bison interface */
extern int  yylex(void);
extern int  yylineno;
void yyerror(const char *msg);
%}

/* -------------------------------------------------------
   Bison declarations
   ------------------------------------------------------- */

/* Value type for tokens / rules */
%union {
    int      ival;
    char    *sval;
    ASTNode *node;
}

/* Token declarations */
%token TOK_BEGIN TOK_END TOK_PRINT
%token TOK_SEMI TOK_ASSIGN
%token TOK_LPAREN TOK_RPAREN
%token <ival> TOK_INTEGER
%token <sval> TOK_IDENT

/* Operator tokens with associativity and precedence (low → high) */
%left  TOK_PLUS TOK_MINUS
%left  TOK_MUL  TOK_DIV

/* Rule return types */
%type <node> stmt expr

%%

/* -------------------------------------------------------
   Grammar Rules
   ------------------------------------------------------- */

/* Top-level: begin ... end */
program
    : TOK_BEGIN stmt_list TOK_END
        {
            parse_result = make_program(
                /* copy the collected statements */
                (ASTNode **)memcpy(
                    malloc(stmt_count * sizeof(ASTNode *)),
                    stmt_buf,
                    stmt_count * sizeof(ASTNode *)
                ),
                stmt_count
            );
        }
    ;

/* Zero or more statements */
stmt_list
    : /* empty */
    | stmt_list stmt
        {
            if (stmt_count >= MAX_STMTS) {
                fprintf(stderr, "[Parser] Too many statements (max %d)\n", MAX_STMTS);
                exit(1);
            }
            stmt_buf[stmt_count++] = $2;
        }
    ;

/* A single statement */
stmt
    : TOK_IDENT TOK_ASSIGN expr TOK_SEMI
        { $$ = make_assign($1, $3); free($1); }
    | TOK_PRINT expr TOK_SEMI
        { $$ = make_print($2); }
    ;

/* Arithmetic expression with precedence */
expr
    : expr TOK_PLUS  expr   { $$ = make_binop(OP_ADD, $1, $3); }
    | expr TOK_MINUS expr   { $$ = make_binop(OP_SUB, $1, $3); }
    | expr TOK_MUL   expr   { $$ = make_binop(OP_MUL, $1, $3); }
    | expr TOK_DIV   expr   { $$ = make_binop(OP_DIV, $1, $3); }
    | TOK_LPAREN expr TOK_RPAREN  { $$ = $2; }
    | TOK_INTEGER           { $$ = make_number($1); }
    | TOK_IDENT             { $$ = make_var($1); free($1); }
    ;

%%

/* -------------------------------------------------------
   Error handler
   ------------------------------------------------------- */
void yyerror(const char *msg) {
    fprintf(stderr, "[Parser Error] Line %d: %s\n", yylineno, msg);
}
