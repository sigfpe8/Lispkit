(STOP)                       ((B C))          ((B C))

(LDC A STOP)                  X                A

(LDC A ATOM STOP)             X                T

(LDC (A) ATOM STOP)           X                F

(LDC (A) CAR STOP)            X                A

(LDC A LDC B CONS STOP)       X               (B.A)

(LDC A LDC B CONS CDR STOP)   X                A

(LDC (A B C) CDR STOP)        X               (B C)

(LDC A LDC B EQ STOP)         X                F

(LDC A LDC A EQ STOP)         X                T

(LDC 3 LDC 5 EQ STOP)         X                F

(LDC 3 LDC 3 EQ STOP)         X                T

(LDC (A) LDC (A) EQ STOP)     X                F

(LDC 271 LDC 127 ADD STOP)    X                398

(LDC 271 LDC -71 ADD STOP)    X                200

(LDC 271 LDC 127 SUB STOP)    X                144

(LDC 271 LDC 127 MUL STOP)    X                34417

(LDC 271 LDC -127 MUL STOP)   X                -34417

(LDC 271 LDC 127 DIV STOP)    X                2

(LDC 271 LDC 127 REM STOP)    X                17

(LDC 271 LDC 127 LEQ STOP)    X                F

(LDC 127 LDC 127 LEQ STOP)    X                T

(LDC 127 LDC 271 LEQ STOP)    X                T

(LDC T SEL (LDC A STOP) (LDC B STOP)) X        A

(LDC F SEL (LDC A STOP) (LDC B STOP)) X        B

(LDC T SEL (LDC A JOIN) (LDC B JOIN) STOP) X   A

(LDC F SEL (LDC A JOIN) (LDC B JOIN) STOP) X   B

(LDF (LDC A) STOP)           ((B C))      ((2 A))

(LDF (LDC A STOP) AP)        ((B C))          A

(LDF (LDC A RTN) AP STOP)    ((B C))          A

(LDF (LD (0 . 0) RTN) AP STOP) ((B C))      (B C)

(LDF (LD (0 . 1) RTN) AP STOP) ((B C) (D E))  (D E)

(LDF (DUM LD (1 . 0) RTN) AP STOP)    ((B C))  (B C)

(LDF (DUM LD (1 . 1) RTN) AP STOP) ((B C) (D E)) (D E)

(DUM LDF (LD (0 . 0) STOP) RAP)((B C))      (B C)
