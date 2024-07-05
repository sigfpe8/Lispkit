#ifndef _LISPKIT_H_
#define _LISPKIT_H_

// A C implementation of the Lispkit system described in the book
//    Functional Programming Application and Implementation
//    Peter Henderson

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// An S-expression is represented as a "tagged pointer", where the
// lower 2 bits encode a tag and the upper 30 bits encode an index
// to the cell array, an index to the symbol array or an integer.
typedef int32_t        sexpr_t;

// A cell holds two sexpr's, a car and a cdr
typedef struct {
    sexpr_t car;
    sexpr_t cdr;
} cell_t;

// The following macros would be in uppercase in typical C code
// but are in lowercase here to follow the book more closely.
#define ivalue(s)       ((int64_t)POINTER((int64_t)s))
#define car(s)          (cell_array[POINTER(s)].car)
#define cdr(s)          (cell_array[POINTER(s)].cdr)

#define TAGMASK         3
#define TAGSHIFT        2
#define PTRBITS         30

// Minimum and maximum Lispkit integers
// Representable in the upper bits of sexpr_t
#define MAXLKINT        ((1 << (PTRBITS-1))-1)
#define MINLKINT        (-(1 << (PTRBITS-1)))

enum tags { integer, symbol, pair };

// Form sexpr given a pointer and a tag
#define TAGPTR(p,t)     (((p) << TAGSHIFT) | t)
// Get tag for a given sexpr
#define TAG(s)          ((s) & TAGMASK)
// Get pointer for a given sexpr
#define POINTER(s)      ((s) >> TAGSHIFT)

// Externs

// cell.c
extern cell_t* cell_array;
extern sexpr_t cell_alloc_pair(sexpr_t car, sexpr_t cdr);
extern void    cell_init();
extern sexpr_t cons(void);

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
