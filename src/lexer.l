%{
/*
 * lexer.l - Lexical Analyzer for DSL Compiler
 *
 * Tokenizes the input DSL source file using Flex.
 * Recognizes keywords, identifiers, integers, operators,
 * assignment, semicolons, and whitespace/comments.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"

%}

/* Suppress unused function warnings */
%option noyywrap
%option yylineno

/* -------------------------------------------------------
   Token Definitions
   ------------------------------------------------------- */

DIGIT       [0-9]
LETTER      [a-zA-Z_]
INTEGER     {DIGIT}+
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
WHITESPACE  [ \t\r]+

%%

    /* --- Keywords --- */
"begin"         { return TOK_BEGIN; }
"end"           { return TOK_END;   }
"print"         { return TOK_PRINT; }

    /* --- Integer Literals --- */
{INTEGER}       {
                    yylval.ival = atoi(yytext);
                    return TOK_INTEGER;
                }

    /* --- Identifiers --- */
{IDENTIFIER}    {
                    yylval.sval = strdup(yytext);
                    return TOK_IDENT;
                }

    /* --- Arithmetic Operators --- */
"+"             { return TOK_PLUS;  }
"-"             { return TOK_MINUS; }
"*"             { return TOK_MUL;   }
"/"             { return TOK_DIV;   }

    /* --- Assignment Operator --- */
"="             { return TOK_ASSIGN; }

    /* --- Statement Terminator --- */
";"             { return TOK_SEMI; }

    /* --- Parentheses --- */
"("             { return TOK_LPAREN; }
")"             { return TOK_RPAREN; }

    /* --- Newlines (track line numbers) --- */
"\n"            { yylineno++; }

    /* --- Whitespace (ignore) --- */
{WHITESPACE}    { /* skip */ }

    /* --- Single-line comments (ignore) --- */
"#"[^\n]*       { /* skip comment */ }

    /* --- Unknown characters --- */
.               {
                    fprintf(stderr, "[Lexer Error] Line %d: Unknown character '%s'\n",
                            yylineno, yytext);
                }

%%
