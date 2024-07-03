#ifndef _LISPKIT_H_
#define _LISPKIT_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// An S-expression is represented as a "tagged pointer", where the
// lower 3 bits encode a tag and the upper 29 bits encode an index
// to the cell array or to the symbol array.
typedef uint32_t        sexpr_t;

// A cell holds two sexpr's, a car and a cdr
typedef struct pair {
    sexpr_t car;
    sexpr_t cdr;
} PAIR;

typedef union {
    int64_t     unint;
    PAIR        upair;
} cell_t;

#define VINT(c)         ((c)->unint)
#define CAR(c)          ((c)->upair.car)
#define CDR(c)          ((c)->upair.cdr)

#define TAGMASK         7
#define TAGSHIFT        3

enum tags { integer, symbol, pair };

// Form sexpr given a pointer and a tag
#define TAGPTR(p,t)     (((p) << TAGSHIFT) | t)
// Get tag for a given sexpr
#define TAG(s)          ((s) & TAGMASK)
// Get pointer for a given sexpr
#define POINTER(s)      ((s) >> TAGSHIFT)

#define CELL(s)         (&cell_array[POINTER(s)])

// Externs

// cell.c
extern cell_t* cell_array;
extern sexpr_t cell_alloc_int(int64_t vint);
extern sexpr_t cell_alloc_pair(sexpr_t car, sexpr_t cdr);
extern void    cell_init();

// sexpr.c
extern sexpr_t nil;
extern sexpr_t getexp(void);
extern sexpr_t getexplist(void);
extern void    putexp(sexpr_t e);
extern void    sexpr_init(void);

// symbol.c
extern int   sym_nil;
extern char* symbol_get(int id);
extern void  symbol_init();
extern int   symbol_intern(char* symb);
extern void  symbol_test();

#endif  // _LISPKIT_H_
