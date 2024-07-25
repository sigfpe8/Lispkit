#ifndef _LISPKIT_H_
#define _LISPKIT_H_

// A C implementation of the Lispkit system described in the book
//    Functional Programming - Application and Implementation
//    Peter Henderson

#include <stdint.h>
#include <stdbool.h>
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

#define TAGMASK         3
#define TAGSHIFT        2
#define PTRBITS         30

// Minimum and maximum Lispkit integers
// Representable in the upper bits of sexpr_t
#define MAXLKINT        ((1 << (PTRBITS-1))-1)
#define MINLKINT        (-(1 << (PTRBITS-1)))

enum tags { integer, symbolic, pair };

// Form sexpr given a pointer and a tag
#define TAGPTR(p,t)     (((p) << TAGSHIFT) | t)
// Get tag for a given sexpr
#define TAG(s)          ((s) & TAGMASK)
// Get pointer for a given sexpr
#define POINTER(s)      ((s) >> TAGSHIFT)

// The following macros would be in uppercase in typical C code
// but are in lowercase here to follow the book more closely.

// Constructors
extern sexpr_t cons(sexpr_t car, sexpr_t cdr);
extern sexpr_t number(int32_t num);
extern sexpr_t symbol(char* sym);

// Type predicates
#define issymbol(s)     (TAG(s) == symbolic)
#define isnumber(s)     (TAG(s) == integer)
#define iscons(s)       (TAG(s) == pair)

// Selectors
#define ivalue(s)       ((int32_t)POINTER((int32_t)s))
#define svalue(s)       (symbol_get(POINTER(s)))
// car/cdr can be used as lvalues
#define car(s)          (cell_array[POINTER(s)].car)
#define cdr(s)          (cell_array[POINTER(s)].cdr)

// SECD Machine state
extern sexpr_t s;      // Stack
extern sexpr_t e;      // Environment
extern sexpr_t c;      // Control
extern sexpr_t d;      // Dump

// Get the current stack pointer (sp)
#define GET_SP(dst)     __asm__ volatile("mov %0, sp\n\t": "=r"(dst));

// Externs

// cell.c
extern cell_t* cell_array;
extern void    cell_init(void);
extern int     cell_alloc(void);

// secd.c
extern sexpr_t exec(sexpr_t fn, sexpr_t args);
extern sexpr_t getobj(sexpr_t src);

// sexpr.c
extern sexpr_t nil;
extern sexpr_t f;
extern sexpr_t t;
extern void    close_input(void);
extern bool    equalexp(sexpr_t e1, sexpr_t e2);
extern sexpr_t enlist(sexpr_t sexp);
extern sexpr_t getexp(void);
extern sexpr_t getexplist(void);
extern FILE*   open_input(char* fname);
extern void    putexp(sexpr_t e);
extern void    sexpr_init(void);

// symbol.c
extern int   symbol_find(char *symb);
extern char* symbol_get(int id);
extern void  symbol_init();
extern int   symbol_intern(char* symb);

// Tests
// Define only one at a time
// #define SYMBOL_TEST
// #define SECD_TEST
#define COMPILER_TEST
// #define FUNCTION_TEST
extern void compiler_test(sexpr_t comp);
extern void function_test(sexpr_t comp);
extern void secd_test(void);
extern void symbol_test();

#endif  // _LISPKIT_H_
