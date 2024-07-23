((CAR (QUOTE A)))
(2 A 10 4 21)

((CDR (QUOTE A)))
(2 A 11 4 21)

((ATOM (QUOTE A)))
(2 A 12 4 21)

((CONS (QUOTE A) (QUOTE B)))
(2 B 2 A 13 4 21)

((ADD (QUOTE A) (QUOTE B)))
(2 A 2 B 15 4 21)

((SUB (QUOTE A) (QUOTE B)))
(2 A 2 B 16 4 21)

((MUL (QUOTE A) (QUOTE B)))
(2 A 2 B 17 4 21)

((DIV (QUOTE A) (QUOTE B)))
(2 A 2 B 18 4 21)

((REM (QUOTE A) (QUOTE B)))
(2 A 2 B 19 4 21)

((EQ (QUOTE A) (QUOTE B)))
(2 A 2 B 14 4 21)

((LEQ (QUOTE A) (QUOTE B)))
(2 A 2 B 20 4 21)

((LAMBDA (X) (QUOTE A)))
(3 (2 A 5) 4 21)

((LAMBDA (X) X))
(3 (1 (0.0) 5) 4 21)

((LAMBDA (X Y) Y))
(3 (1 (0.1) 5) 4 21)

(((LAMBDA (X) X) (QUOTE A)))
(2 NIL 2 A 13 3 (1 (0.0) 5) 4 4 21)

((LET X (X QUOTE A)))
(2 NIL 2 A 13 3 (1 (0.0) 5) 4 4 21)

((LETREC X (X QUOTE A)))
(6 2 NIL 2 A 13 3 (1 (0.0) 5) 7 4 21)

((IF (QUOTE A) (QUOTE B) (QUOTE C)))
(2 A 8 (2 B 9) (2 C 9) 4 21)
