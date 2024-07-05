#include "lispkit.h"

#define NUMCELLS        (1024*10)

cell_t *cell_array;
int free_cell;

void cell_init()
{
    cell_array = malloc(NUMCELLS * sizeof(cell_t));
    if (!cell_array) {
        fprintf(stderr, "Could not allocate cell array\n");
        exit(1);
    }

    int i;

    cell_t *pc = cell_array + 1;
    for (i = 1; i < NUMCELLS-1; ++i) {
        CDR(pc) = i + 1;
        ++pc;
    }
    CDR(pc) = 0;
    free_cell = 1;
}

static int cell_alloc()
{
    int ind;

    if (!(ind = free_cell)) {
        fprintf(stderr, "Cell heap is full\n");
        exit(1);
    }

    free_cell = CDR(&cell_array[ind]);
    return ind;
}

sexpr_t cell_alloc_pair(sexpr_t car, sexpr_t cdr)
{
    int ind = cell_alloc();
    cell_t *pc = &cell_array[ind];
    CAR(pc) = car;
    CDR(pc) = cdr;

    return TAGPTR(ind,pair);
}
