#include "lispkit.h"

extern sexpr_t* initial_sp;

static sexpr_t read_compiler(void);

int main()
{
    // Beginning of the cpu stack; used by the GC
    GET_SP(initial_sp);

    printf("Hello, Lispkit!\n");

    symbol_init();
    cell_init();
    sexpr_init();

#ifdef  SYMBOL_TEST
    symbol_test();
    return 0;
#endif

#ifdef  SECD_TEST
    secd_test();
    return 0;
#endif

    sexpr_t comp = read_compiler();

#ifdef  COMPILER_TEST
    compiler_test(comp);
    return 0;
#endif

#ifdef  FUNCTION_TEST
    function_test(comp);
    return 0;
#endif

    open_input("stdin");
    while (!feof(stdin)) {
        printf("> ");
        sexpr_t fun = getexp();
        if (fun == nil) continue;
        putexp(fun); printf("\n");
        sexpr_t obj = exec(comp, fun);
        putexp(obj); printf("\n");
    }

    return 0;
}

static sexpr_t read_compiler(void)
{
    open_input("../src/compiler.secd-obj");
    sexpr_t c = getexp();
    close_input();
    return c;
}

