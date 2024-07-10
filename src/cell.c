#include <assert.h>

#include "lispkit.h"

// Only cons() expressions occupy cells.
// Integers and symbols are self-contained and don't need cells.

// NUMCELLS must be a multiple of 8 because the GC bit_array[]
// stores bits as 8 bits per bytes.
//#define NUMCELLS        (1024*10)
#define NUMCELLS        (40)
#define NUMGCBYTES      (NUMCELLS/8)

#define MARK(b)         (bit_array[b / 8] |= (1 << (b & 7)))
#define MARKED(b)       (bit_array[b / 8] & (1 << (b & 7)))

cell_t*  cell_array;
uint8_t* bit_array;
int free_cell;

sexpr_t* initial_sp;
sexpr_t* final_sp;

static void cell_mark(sexpr_t sexp);
static void cell_mark_cpu_stack(void);
static void cell_collect(void);
static void cell_gc(void);

void cell_init(void)
{
    cell_array = malloc(NUMCELLS * sizeof(cell_t));
    if (!cell_array) {
        fprintf(stderr, "Could not allocate cell array\n");
        exit(1);
    }

    bit_array = malloc(NUMGCBYTES);
    if (!bit_array) {
        fprintf(stderr, "Could not allocate bit array\n");
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

// Allocate 1 cons cell and return its index
int cell_alloc(void)
{
    int ind;

    if (!(ind = free_cell)) {
        // If the free list is empty, call the garbage collector
        cell_gc();
        // If after the GC there are no free cells, stop the process
        if (!(ind = free_cell)) {
            fprintf(stderr, "GC could not free any cell\n");
            exit(1);
        }
    }

    free_cell = cell_array[ind].cdr;
    return ind;
}

// The garbage collector
void cell_gc(void)
{
    GET_SP(final_sp);

    // Clear the bit array
    memset(bit_array, 0, NUMGCBYTES);
    // Mark cell 0
    MARK(0);
    // Mark all cells reached from the SECD registers
    cell_mark(s);
    cell_mark(e);
    cell_mark(c);
    cell_mark(d);
    // No need to mark nil, t and f because they are
    // are symbols and don't occupy cells

    // Many callers may have pending sexpr_t's on the stack.
    // So we need to scan it for possible candidates.
    // All active sexpr_t's will be found but some false
    // positives may also be found and will be marked too.
    // This is a conservative collector, meaning it might
    // free less cells than are actually unused.
    cell_mark_cpu_stack();

    // Now return all cells which are not marked to the free list
    cell_collect();
}

// Scan the cpu stack marking live sexpr_t's
static void cell_mark_cpu_stack(void)
{
    // The stack grows downwards
    assert(initial_sp > final_sp);

    for (sexpr_t* ps = final_sp; ps < initial_sp; ++ps) {
        sexpr_t se = *ps;
        if (se > 0 && iscons(se) && POINTER(se) < NUMCELLS) {
            cell_mark(se);
        }
    }
}

// Mark one cell as live
static void cell_mark(sexpr_t sexp)
{
    if (iscons(sexp)) {
        int ind = POINTER(sexp);
        if (!MARKED(ind)) { // Avoid cycles
            MARK(ind);
            cell_mark(car(sexp));
            cell_mark(cdr(sexp));
        }
    }
}

// Return all cells not marked (live) to the free list
static void cell_collect(void)
{
    int ind = 0;
    int nfree = 0;

    // Scan all bytes in bit_array[]
    for (int by = 0; by < NUMGCBYTES; ++by) {
        uint8_t byte = bit_array[by];
        if (byte == 0xFF) {
            // All the cells in this byte are in use; just skip it
            ind += 8;
        } else {
            // Scan all bits in this byte
            for (int bi = 0; bi < 8; ++bi) {
                if (!(byte & (1 << bi))) {
                    cell_array[ind].cdr = free_cell;
                    free_cell = ind;
                    ++nfree;
                }
                ++ind;
            }
        }
    }
//    printf("GC freed %d cells\n", nfree);
}
