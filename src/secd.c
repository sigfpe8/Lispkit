#include <stdio.h>

#include "lispkit.h"

enum opcode {
    // Control operations
    NOP,        //  0 = no operation
    LD,         //  1 = load
    LDC,        //  2 = load constant
    LDF,        //  3 = load function
    AP,         //  4 = apply function
    RTN,        //  5 = return
    DUM,        //  6 = create dummy environment
    RAP,        //  7 = recursive apply
    SEL,        //  8 = select subcontrol
    JOIN,       //  9 = rejoin main control

    // Operations on top stack item
    CAR,        // 10 = take car of item on top of stack
    CDR,        // 11 = take cdr of item on top of stack
    ATOM,       // 12 = apply atom predicate to top stack item

    // Operations on top two stack items
    CONS,       // 13 = form cons of top two stack items
    EQ,         // 14 = apply eq predicate to top two stack items
    ADD,        // 15 = add top two stack items
    SUB,        // 16 = subtract top item from top-1 stack item
    MUL,        // 17 = multiply top two stack items
    DIV,        // 18 = divide top-1 item by top stack item
    REM,        // 19 = rest of division of top-1 by top stack item
    LEQ,        // 20 = apply predicate (top-1) <= (top)
    STOP        // 21 = stop
};

// Machine state
static sexpr_t s;      // Stack
static sexpr_t e;      // Environment
static sexpr_t c;      // Control
static sexpr_t d;      // Dump

// Temporary register
static sexpr_t w;

static int RUN;

sexpr_t exec(sexpr_t fn, sexpr_t args)
{
    RUN = 1;
    s = cons(args, nil);
    e = nil;
    c = fn;
    d = nil;

    while (RUN) {
        switch (ivalue(car(c))) {
        case NOP:
            break;
        case LD:
            break;
        case LDC:
            break;
        case LDF:
            break;
        case AP:
            break;
        case RTN:
            break;
        case DUM:
            break;
        case RAP:
            break;
        case SEL:
            break;
        case JOIN:
            break;
        case CAR:
            // ((a.b).s) e (CAR.c) d -> (a.s) e c d
            s = cons(car(car(s)), cdr(s));
            c = cdr(c);
            break;
        case CDR:
            break;
        case ATOM:
            break;
        case CONS:
            break;
        case EQ:
            break;
        case ADD:
            break;
        case SUB:
            break;
        case MUL:
            break;
        case DIV:
            break;
        case REM:
            break;
        case LEQ:
            break;
        case STOP:
            RUN = 0;
            break;
        default:
            fprintf(stderr,"exec(): invalid opcode: %d\n", ivalue(car(c)));
            RUN = 0;
            break;
        }
    }
    return 0;
}

