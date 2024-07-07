#include "lispkit.h"

int main()
{
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

