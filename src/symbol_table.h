/*
 * symbol_table.h - Symbol Table interface
 *
 * A simple hash-table-based symbol table that stores
 * declared integer variables for semantic checking.
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define SYM_TABLE_SIZE 64   /* Number of hash buckets */

/* -------------------------------------------------------
   Symbol Entry
   ------------------------------------------------------- */
typedef struct SymEntry {
    char           *name;       /* Variable name          */
    int             defined;    /* 1 = assigned at least once */
    struct SymEntry *next;      /* Chained entry (collision) */
} SymEntry;

/* -------------------------------------------------------
   Symbol Table
   ------------------------------------------------------- */
typedef struct {
    SymEntry *buckets[SYM_TABLE_SIZE];
} SymbolTable;

/* -------------------------------------------------------
   API
   ------------------------------------------------------- */

/* Allocate and initialise a new symbol table */
SymbolTable *sym_table_create(void);

/* Free all resources */
void sym_table_destroy(SymbolTable *table);

/* Declare a variable (mark as defined) */
void sym_table_insert(SymbolTable *table, const char *name);

/* Look up a variable; returns its entry or NULL */
SymEntry *sym_table_lookup(SymbolTable *table, const char *name);

/* Print all symbols (debug) */
void sym_table_dump(SymbolTable *table);

#endif /* SYMBOL_TABLE_H */
