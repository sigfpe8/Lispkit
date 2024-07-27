# The Lispkit

The book [Functional Programming Application and Implementation](https://www.goodreads.com/book/show/5838062-functional-programming-application-and-implementation) by Peter Henderson was published in 1980 and may look a bit outdated after more than 40 years. However, I think it still provides a solid foundation for many concepts. I especially like his treatment of the SECD machine and associated compiler for a small language called *Lispkit Lisp*. In the final chapters of the book he describes in pseudo-Algol an implementation of such a virtual machine. The whole system is known as the *Lispkit*.

This repository contains my implementation of the Lispkit in good old C. This is a system designed for learning about functional programming and the implementation of functional languages. It is not intended for running real code.

## Building the `Lispkit` on macOS / Apple Silicon

Currently you can build the system in order to perform certain tests. Other tests can be added in the future in order to explore some of the exercises proposed in the text.

### Decide which test mode you want to build

 At the end of file `lispkit.h` select one (and only 1) of the test macros, say

    #define COMPILER_TEST

### Create the build directory and run `cmake`
```
% mkdir build
% cd build
% cmake ..
-- The C compiler identification is AppleClang 15.0.0.15000309
...
```
### Run `make`

```
% make
[ 14%] Building C object CMakeFiles/lispkit.dir/src/cell.c.o
...
```

### Run the `Lispkit`

    % ./lispkit
    Hello, Lispkit!

    Testing the compiler
    Test: ((CAR (QUOTE A))) Success!
    Test: ((CDR (QUOTE A))) Success!
    Test: ((ATOM (QUOTE A))) Success!
    Test: ((CONS (QUOTE A) (QUOTE B))) Success!
    Test: ((ADD (QUOTE A) (QUOTE B))) Success!
    Test: ((SUB (QUOTE A) (QUOTE B))) Success!
    Test: ((MUL (QUOTE A) (QUOTE B))) Success!
    Test: ((DIV (QUOTE A) (QUOTE B))) Success!
    Test: ((REM (QUOTE A) (QUOTE B))) Success!
    Test: ((EQ (QUOTE A) (QUOTE B))) Success!
    Test: ((LEQ (QUOTE A) (QUOTE B))) Success!
    Test: ((LAMBDA (X) (QUOTE A))) Success!
    Test: ((LAMBDA (X) X)) Success!
    Test: ((LAMBDA (X Y) Y)) Success!
    Test: (((LAMBDA (X) X) (QUOTE A))) Success!
    Test: ((LET X (X QUOTE A))) Success!
    Test: ((LETREC X (X QUOTE A))) Success!
    Test: ((IF (QUOTE A) (QUOTE B) (QUOTE C))) Success!

    Compiling the compiler: Success!

    All tests passed!

    # of allocations: 71700
    # of GC calls: 9


## Hardware dependencies

There is a non-portable macro used to capture the machine stack pointer
```
// Get the current stack pointer (sp)
#define GET_SP(dst)     __asm__ volatile("mov %0, sp\n\t": "=r"(dst));
```

It is used only two times, first at the start of `main` when the stack is empty, and then at the start of the garbage collector. With this information we can scan the machine stack looking for possible live `cons` pointers held in variables of type `sexpr_t` in intermediate functions.

The above in-line assembly syntax is valid only for Clang/GCC on ARM64 chips such as the Apple M series. For `x86` and for MS compilers it would have to be adapted.

