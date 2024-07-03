#include "lispkit.h"

int main()
{
    printf("Hello, Lispkit!\n");

    symbol_init();
    cell_init();
    sexpr_init();

    while (!feof(stdin)) {
        printf("> ");
        sexpr_t e = getexp();
        putexp(e);
        printf("\n");
    }

    return 0;
}
