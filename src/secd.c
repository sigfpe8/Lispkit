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

sexpr_t exec(sexpr_t fn, sexpr_t args)
{
    int i, n;
    int RUN;

    // Machine state
    sexpr_t s;      // Stack
    sexpr_t e;      // Environment
    sexpr_t c;      // Control
    sexpr_t d;      // Dump
    // Temp registers
    sexpr_t w, z;

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
            // s e (LD z.c) d -> (locate(i,e).s) e c d
            // where z = (b.n) b,n integers
            w = e;
            z = car(cdr(c));
            n = ivalue(car(z));
            for (int i = 1; i <= n; ++i)
                w = cdr(w);
            w = car(w);
            n = ivalue(cdr(z));
            for (int i = 1; i <= n; ++i)
                w = cdr(w);
            w = car(w);
            s = cons(w,s);
            c = cdr(cdr(c));
            break;
        case LDC:
            // s e (LDC x.c) d -> (x.s) e c d
            s = cons(car(cdr(c)), s);
            c = cdr(cdr(c));
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
            // ((a.b).s) e (CAR.c) d -> (b.s) e c d
            s = cons(cdr(car(s)), cdr(s));
            c = cdr(c);
            break;
        case ATOM:
            s = cons(iscons(car(s)) ? f : t, cdr(s));
            c = cdr(c);
            break;
        case CONS:
            // (a b.s) e (CONS.c) d -> ((a.b).s) e c d
            s = cons(cons(car(s), car(cdr(s))), cdr(cdr(s)));
            c = cdr(c);
            break;
        case EQ:
            // (a b.s) e (EQ.c) d -> (b = a.s) e c d
            s = cons(car(cdr(s)) == car(s) ? t : f, cdr(cdr(s)));
            c = cdr(c);
            break;
        case ADD:
            // (a b.s) e (ADD.c) d -> (b + a.s) e c d
            s = cons(number(ivalue(car(cdr(s))) + ivalue(car(s))), cdr(cdr(s)));
            c = cdr(c);
            break;
        case SUB:
            // (a b.s) e (SUB.c) d -> (b - a.s) e c d
            s = cons(number(ivalue(car(cdr(s))) - ivalue(car(s))), cdr(cdr(s)));
            c = cdr(c);
            break;
        case MUL:
            // (a b.s) e (MUL.c) d -> (b * a.s) e c d
            s = cons(number(ivalue(car(cdr(s))) * ivalue(car(s))), cdr(cdr(s)));
            c = cdr(c);
            break;
        case DIV:
            // (a b.s) e (DIV.c) d -> (b - a.s) e c d
            s = cons(number(ivalue(car(cdr(s))) / ivalue(car(s))), cdr(cdr(s)));
            c = cdr(c);
            break;
        case REM:
            // (a b.s) e (REM.c) d -> (b % a.s) e c d
            s = cons(number(ivalue(car(cdr(s))) % ivalue(car(s))), cdr(cdr(s)));
            c = cdr(c);
            break;
        case LEQ:
            // (a b.s) e (LEQ.c) d -> (b <= a.s) e c d
            s = cons(ivalue(car(cdr(s))) <= ivalue(car(s)) ? t : f, cdr(cdr(s)));
            c = cdr(c);
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
    return car(s);  // Top of the stack
}

