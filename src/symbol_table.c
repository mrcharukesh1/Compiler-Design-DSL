/*
 * symbol_table.c - Symbol Table implementation
 *
 * Uses a simple djb2 hash function for bucket selection.
 * Collisions are resolved by chaining (linked list per bucket).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

/* -------------------------------------------------------
   djb2 hash function
   ------------------------------------------------------- */
static unsigned int hash(const char *name) {
    unsigned int h = 5381;
    while (*name)
        h = ((h << 5) + h) + (unsigned char)(*name++);
    return h % SYM_TABLE_SIZE;
}

/* -------------------------------------------------------
   sym_table_create
   ------------------------------------------------------- */
SymbolTable *sym_table_create(void) {
    SymbolTable *t = (SymbolTable *)calloc(1, sizeof(SymbolTable));
    if (!t) {
        fprintf(stderr, "[SymTable] Out of memory\n");
        exit(EXIT_FAILURE);
    }
    return t;
}

/* -------------------------------------------------------
   sym_table_destroy
   ------------------------------------------------------- */
void sym_table_destroy(SymbolTable *table) {
    for (int i = 0; i < SYM_TABLE_SIZE; i++) {
        SymEntry *e = table->buckets[i];
        while (e) {
            SymEntry *next = e->next;
            free(e->name);
            free(e);
            e = next;
        }
    }
    free(table);
}

/* -------------------------------------------------------
   sym_table_insert: add or update a variable entry
   ------------------------------------------------------- */
void sym_table_insert(SymbolTable *table, const char *name) {
    unsigned int idx = hash(name);
    SymEntry    *e   = table->buckets[idx];

    /* Check if already exists */
    while (e) {
        if (strcmp(e->name, name) == 0) {
            e->defined = 1;
            return;
        }
        e = e->next;
    }

    /* Create new entry */
    SymEntry *ne = (SymEntry *)malloc(sizeof(SymEntry));
    if (!ne) { fprintf(stderr, "[SymTable] Out of memory\n"); exit(1); }
    ne->name    = strdup(name);
    ne->defined = 1;
    ne->next    = table->buckets[idx];
    table->buckets[idx] = ne;
}

/* -------------------------------------------------------
   sym_table_lookup
   ------------------------------------------------------- */
SymEntry *sym_table_lookup(SymbolTable *table, const char *name) {
    unsigned int idx = hash(name);
    SymEntry    *e   = table->buckets[idx];

    while (e) {
        if (strcmp(e->name, name) == 0)
            return e;
        e = e->next;
    }
    return NULL;
}

/* -------------------------------------------------------
   sym_table_dump: print all symbols (debug)
   ------------------------------------------------------- */
void sym_table_dump(SymbolTable *table) {
    printf("[SymTable] Contents:\n");
    for (int i = 0; i < SYM_TABLE_SIZE; i++) {
        SymEntry *e = table->buckets[i];
        while (e) {
            printf("  %s (defined=%d)\n", e->name, e->defined);
            e = e->next;
        }
    }
}
