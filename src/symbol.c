#include "lispkit.h"


// Symbol table
//
// The symbol table has 3 parts:
//  1. The hash table, indexed by the symbol hash
//  2. The collision area, where symbols with the same hash are chained
//  3. The string space, where the strings are copied
//
// For convenience, the first two areas are allocated from the same array,
// with the first SYMHASHSIZE entries being the hash table and the following
// ones the collion area. The symbol id is its index in this array.
//
// The string space could be allocated from a separate char array, but for now
// we're just using strdup() to copy the symbols to the heap.

#define SYMHASHSIZE     256     // Must be a power of 2
#define COLAREASIZE     1024

typedef struct {
    char                *name;  // Symbol name
    uint32_t            next;   // Index to collision chain
    uint32_t            len;    // Symbol length
} symbol_entry;

static symbol_entry *hash_array;
static symbol_entry *hash_chain;
static int next_col_entry; // Next entry in the collision area

void symbol_init()
{
    hash_array = malloc((SYMHASHSIZE+COLAREASIZE) * sizeof(symbol_entry));
    if (!hash_array) {
        fprintf(stderr, "Could not allocate symbol table\n");
        exit(1);
    }

    // Clear hash table
    memset(hash_array, 0, SYMHASHSIZE * sizeof(symbol_entry));

    // Collision area is just after the hash table
    next_col_entry = SYMHASHSIZE;
}

// Calculate symbol hash
int symbol_hash(int len, char *symb)
{
    int hash = 0;
    char *pc = symb;

    while (*pc) {
        hash += *pc++;
    }

    hash = (hash >> 8) ^ hash;
    
    return hash & (SYMHASHSIZE - 1);
}

// Return symbol id or -1 if not in table
int symbol_find(char *symb)
{
    int len = strlen(symb);
    int id  = symbol_hash(len, symb);
    symbol_entry *ps;

    do {
        ps = &hash_array[id];
        if (ps->len == len && ps->name && !strcmp(ps->name, symb))
            return id;  // Found symbol, return its id
        // Not this entry, try the next one
        id = ps->next;
    } while (id);

    return -1;
}

// Return symbol name given its id
char* symbol_get(int id)
{
    symbol_entry *ps = &hash_array[id];
    return ps->name;
}

// Internalize symbol (map symbol to int id)
int symbol_intern(char* symb)
{
    int len = strlen(symb);
    int id  = symbol_hash(len, symb);
    symbol_entry *ps = &hash_array[id];

    // If hash table entry is empty, just use it
    if (!ps->name) {
        ps->name = strdup(symb);
        ps->next = 0;
        ps->len  = len;
        return id;
    }

    // Search collision chain
    do {
        ps = &hash_array[id];
        if (ps->len == len && !strcmp(ps->name, symb))
            return id;  // Found symbol, return its id
        // Not this entry, try the next one
        id = ps->next;
    } while (id);

    // Symbol not found, allocate a new collision entry
    id = next_col_entry;
    if (id >= SYMHASHSIZE + COLAREASIZE) {
        fprintf(stderr, "Symbol table is full\n");
        exit(1);
    }
    ++next_col_entry;
    ps->next = id;
    ps = &hash_array[id];
    ps->name = strdup(symb);
    ps->next = 0;
    ps->len  = len;

    return id;
}

