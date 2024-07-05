#include "lispkit.h"

#define NUMCELLS        (1024*10)

cell_t *cell_array;
int free_cell;

void cell_init(void)
{
    cell_array = malloc(NUMCELLS * sizeof(cell_t));
    if (!cell_array) {
        fprintf(stderr, "Could not allocate cell array\n");
        exit(1);
    }

    // Chain the fee list using the cdr of each cell
    // Don't use cell 0 because 0 is used as the list terminator
    cell_t *pc = cell_array + 1;
    for (int i = 1; i < NUMCELLS-1; ++i) {
        pc->cdr = i + 1;
        ++pc;
    }
    // Mark end of the fee list
    pc->cdr = 0;
    free_cell = 1;
}

int cell_alloc(void)
{
    int ind;

    if (!(ind = free_cell)) {
        fprintf(stderr, "Cell heap is full\n");
        exit(1);
    }

    free_cell = cell_array[ind].cdr;
    return ind;
}
