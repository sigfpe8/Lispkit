#include "lispkit.h"

extern sexpr_t* initial_sp;

int main()
{
    // Beginning of the cpu stack; used by the GC
    GET_SP(initial_sp);

    printf("Hello, Lispkit!\n");

    symbol_init();
    cell_init();
    sexpr_init();

#ifdef  SECD_TEST
   secd_test();
#else
    while (!feof(stdin)) {
        printf("> ");
        sexpr_t e = getexp();
        putexp(e);
        printf("\n");
    }
#endif

    return 0;
}

