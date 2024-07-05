#ifndef _LISPKIT_H_
#define _LISPKIT_H_

// A C implementation of the Lispkit system described in the book
//    Functional Programming - Application and Implementation
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

// car/cdr can be used as lvalues
#define car(s)          (cell_array[POINTER(s)].car)
#define cdr(s)          (cell_array[POINTER(s)].cdr)
// but ivalue/svalue cannot
#define ivalue(s)       ((int32_t)POINTER((int32_t)s))
#define svalue(s)       (symbol_get(POINTER(s)))

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

// Type predicates
#define issymbol(s)     (TAG(s) == symbol)
#define isnumber(s)     (TAG(s) == integer)
#define iscons(s)       (TAG(s) == pair)

// Externs

// cell.c
extern cell_t* cell_array;
extern void    cell_init();
extern sexpr_t cons(sexpr_t car, sexpr_t cdr);
extern sexpr_t make_cons(void);

// sexpr.c
extern sexpr_t nil;
extern sexpr_t f;
extern sexpr_t t;
extern sexpr_t getexp(void);
extern sexpr_t getexplist(void);
extern void    putexp(sexpr_t e);
extern void    sexpr_init(void);

// symbol.c
extern char* symbol_get(int id);
extern void  symbol_init();
extern int   symbol_intern(char* symb);
extern void  symbol_test();

#endif  // _LISPKIT_H_
