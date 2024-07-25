#include <ctype.h>
#include <stdio.h>

#include "lispkit.h"

// Parser and scanner

typedef enum {
    alphanum, numeric, lparen, rparen, dot, other, eol, eof
} token_t;

// Expression input stream
static FILE *expin;

static token_t nextToken;
static int  nextChar;
static char TokString[128];
static int  ti;

static void getnxtchar(void);
static void gettoken(void);
static void newline(void);
static void storechar(void);
static bool equalexplist(sexpr_t e1, sexpr_t e2);

sexpr_t nil;
sexpr_t t;
sexpr_t f;

// Constructors
sexpr_t symbol(char* sym)
{
    int32_t ind;

    ind = symbol_intern(sym);
    if (ind > MAXLKINT) {
        fprintf(stderr, "symbol(): symbol index is too large: %d\n", ind);
        exit(1);
    }

    return TAGPTR(ind, symbolic);
}

sexpr_t number(int32_t num)
{
    if (num < MINLKINT) {
        fprintf(stderr, "number(): integers must be >= %d\n", MINLKINT);
        num = MINLKINT;
    } else if (num > MAXLKINT) {
        fprintf(stderr, "number(): integers must be <= %d\n", MAXLKINT);
        num = MAXLKINT;
    }

    return TAGPTR(num, integer);
}

sexpr_t cons(sexpr_t car, sexpr_t cdr)
{
    int ind = cell_alloc();
    cell_t *pc = &cell_array[ind];
    pc->car = car;
    pc->cdr = cdr;

    return TAGPTR(ind,pair);
}

void sexpr_init(void)
{
    // Predefined symbols
    nil = symbol("NIL");
      f = symbol("F");
      t = symbol("T");

    // Read from stdin by default
    open_input("stdin");
}

// Open new input stream
FILE* open_input(char* fname)
{
    // Close previous stream if not stdin
    close_input();

    // stdin doesn't need to be opened
    if (!strcmp(fname, "stdin")) {
        expin = stdin;
    } else {
        if (!(expin = fopen(fname, "r"))) {
            fprintf(stderr, "Cound not open input stream `%s`\n", fname);
            exit(1);
        }
    }

    // Force gettoken() to read new line
    nextToken = eol;
    nextChar = 12;

    return expin;
}

void close_input(void)
{
    // Might already be closed; ignore error
    if (expin && expin != stdin) // Don't close stdin
        fclose(expin);
    expin = 0;
}

// Read in an S-expression
sexpr_t getexp(void)
{
    sexpr_t e;

    newline();

    if (nextToken == lparen) {
        gettoken();     // Skip (
        e = getexplist();
        newline();
        if (nextToken != rparen) {
            fprintf(stderr, "getexp(): expected ')'\n");
            e = nil;
        }
    } else if (nextToken == numeric) {
        e = number(strtoll(TokString,0,10));
    } else if (nextToken == alphanum) {
        e = symbol(TokString);
    } else if (nextToken == eof) {
        e = nil;
    } else {
        fprintf(stderr, "getexp(): invalid token: %s\n", TokString);
        e = nil;
    }

    gettoken(); // Look ahead next token
    return e;
}

// Read in a list of S-expressions
sexpr_t getexplist(void)
{
    sexpr_t car, cdr;

    newline();
    car = getexp();
    newline();

    if (nextToken == dot) {
        gettoken(); // Skip .
        cdr = getexp();
    } else if (nextToken == rparen) {
        cdr = nil;
    } else
        cdr = getexplist();

    return cons(car, cdr);
}

// Output an S-expression
void putexp(sexpr_t e)
{
    switch (TAG(e)) {
    case integer:
        printf("%d", ivalue(e));
        break;
    case symbolic:
        printf("%s", symbol_get(POINTER(e)));
        break;
    case pair:
        printf("(");
        while (iscons(e)) {
            putexp(car(e));
            e = cdr(e);
            if (iscons(e)) printf(" ");
        }
        if (e != nil) {
            printf(".");
            putexp(e);
        }
        printf(")");
        break;
    default:
        fprintf(stderr, "putexp(): invalid tag: %d\r\n", TAG(e));
        break;
    }
}

// Compares two S-expressions
// Return true if they have the same content
bool equalexp(sexpr_t e1, sexpr_t e2)
{
    // If same expression, they're equal
    if (e1 == e2) return true;

    int tag;

    // Different types cannot be equal
    if ((tag = TAG(e1)) != TAG(e2)) return false;

    // Different lists can have the same content
    if (tag != pair) return false;

    return equalexplist(e1, e2);
}

static bool equalexplist(sexpr_t e1, sexpr_t e2)
{
    if (!equalexp(car(e1),car(e2))) return false;

    sexpr_t cdr1 = cdr(e1);
    sexpr_t cdr2 = cdr(e2);
    if (cdr1 == nil) return cdr2 == nil;
    if (cdr2 == nil) return false;

    if (!iscons(cdr1) || !iscons(cdr2)) return equalexp(cdr1,cdr2);
    return equalexplist(cdr1,cdr2);
}

// Add item to a new list
sexpr_t enlist(sexpr_t sexp)
{
    return cons(sexp, nil);
}

// Reads a token and updates nextToken with it
static void gettoken(void)
{
    // Skip white space
    while (nextChar <= 32) {
        if (feof(expin)) {
            nextToken = eof;
            return;
        }
        if (nextChar == '\n' || nextChar == '\r') {
            nextChar = 0;
            nextToken = eol;
            return;
        }
        getnxtchar();
    }

    // Number?
    if (isdigit(nextChar) || nextChar == '-') {
        ti = 0;
        if (nextChar == '-') {
            storechar();
            getnxtchar();
            if (!isdigit(nextChar)) {
                fprintf(stderr, "gettoken(): expected digit after '-'\n");
                nextToken = eof;
                return;
            }
        }
        while (isdigit(nextChar)) {
            storechar();
            getnxtchar();
        }
        TokString[ti] = 0;
        nextToken = numeric;
        return;
    }

    // Symbol?
    if (isalpha(nextChar)) {
        ti = 0;
        while (isalnum(nextChar)) {
            storechar();
            getnxtchar();
        }
        TokString[ti] = 0;
        nextToken = alphanum;
        return;

    }

    // Punctuation
    switch (nextChar) {
    case '(': nextToken = lparen; break;
    case ')': nextToken = rparen; break;
    case '.': nextToken = dot;    break;
    default:  nextToken = other;  break;
    }

    TokString[0] = nextChar;
    TokString[1] = 0;
    getnxtchar();
}

// Concatenate next char to the token string
// Check for space, leave room for end of string
static void storechar(void)
{
    if (ti < (sizeof(TokString)-1))
        TokString[ti++] = nextChar;
    else {
        TokString[ti] = 0;
        fprintf(stderr, "nextToken is too long: %s\n", TokString);
    }
}

// If we are at the end of a line,
// force a new line to be read in
static void newline(void)
{
    while (nextToken == eol)
        gettoken();
}

// Read next character
static void getnxtchar(void)
{
    nextChar = getc(expin);
}
