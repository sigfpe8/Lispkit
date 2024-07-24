#include "lispkit.h"

extern FILE *expin;

#ifdef  SYMBOL_TEST
void symbol_test()
{
    int errs = 0;

    printf("\nTesting symbol functions\n");

    int id_for   = symbol_intern("for");
    int id_if    = symbol_intern("if");
    int id_int   = symbol_intern("int");
    int id_long  = symbol_intern("long");
    int id_void  = symbol_intern("void");
    int id_while = symbol_intern("while");
    int id_hwile = symbol_intern("hwile");
    int id_wheli = symbol_intern("wheli");

    printf("id_for    = %d\n", id_for);
    printf("id_if     = %d\n", id_if);
    printf("id_int    = %d\n", id_int);
    printf("id_long   = %d\n", id_long);
    printf("id_void   = %d\n", id_void);
    printf("id_while  = %d\n", id_while);
    printf("id_hwile  = %d\n", id_hwile);
    printf("id_wheli  = %d\n", id_wheli);

    printf("symbol_find(for)   = %d\n", symbol_find("for"));
    printf("symbol_find(if)    = %d\n", symbol_find("if"));
    printf("symbol_find(int)   = %d\n", symbol_find("int"));
    printf("symbol_find(void)  = %d\n", symbol_find("void"));
    printf("symbol_find(while) = %d\n", symbol_find("while"));
    printf("symbol_find(hwile) = %d\n", symbol_find("hwile"));
    printf("symbol_find(wheli) = %d\n", symbol_find("wheli"));

    printf("symbol_find(char)  = %d\n", symbol_find("char"));
    printf("symbol_find(short) = %d\n", symbol_find("short"));
    printf("symbol_find(struct)= %d\n", symbol_find("struct"));
}
#endif

#ifdef  SECD_TEST
void secd_test(void)
{
    FILE *savein = expin;
    int errs = 0;

    expin = fopen("../tests/secd-tests.lisp", "r");
    if (!expin) {
        fprintf(stderr, "Could not open secd-tests.lisp\n");
        exit(1);
    }

    printf("\nTesting the SECD VM\n");
    while (!feof(expin)) {
        sexpr_t src  = getexp();        // SECD "ASM" source
        if (src == nil) break;          // EOF?
        sexpr_t obj = getobj(src);      // SECD "OBJ" code
        sexpr_t args = getexp();        // Arguments
        sexpr_t exp  = getexp();        // Expected result
        sexpr_t res = exec(obj, args);  // Result

        printf("Test: "); putexp(src);
        if (equalexp(exp,res)) printf(" Success!\n");
        else {
            printf(" ## ERROR ##");
            printf("\n  Expected: "); putexp(exp);
            printf("\n  Got:      "); putexp(res);
            printf("\n");
            ++errs;
        }
    }

    if (!errs) printf("\nAll tests passed!\n");
    else {
        printf("\nThere %s %d error%s!\n",
            errs == 1 ? "was" : "were", errs,
            errs == 1 ? "" : "s");
    }
}

#endif  // SECD_TEST

#ifdef  COMPILER_TEST
void compiler_test(sexpr_t comp)
{
    FILE *savein = expin;
    int errs = 0;

    expin = fopen("../tests/compiler-tests.lisp", "r");
    if (!expin) {
        fprintf(stderr, "Could not open compiler-tests.lisp\n");
        exit(1);
    }

    printf("\nTesting the compiler\n");
    while (!feof(expin)) {
        sexpr_t src = getexp();         // Lisp source to be compiled
        sexpr_t exp = getexp();         // Expected object code
        if (src == nil) break;          // EOF?
        sexpr_t obj = exec(comp, src);  // Result object code

        printf("Test: "); putexp(src);
        if (equalexp(exp,obj)) printf(" Success!\n");
        else {
            printf(" ## ERROR ##");
            printf("\n  Expected: "); putexp(exp);
            printf("\n  Got:      "); putexp(obj);
            printf("\n");
            ++errs;
        }
    }

    if (!errs) printf("\nAll tests passed!\n");
    else {
        printf("\nThere %s %d error%s!\n",
            errs == 1 ? "was" : "were", errs,
            errs == 1 ? "" : "s");
    }

   expin = savein;
}
#endif
