(LETREC APPEND
  (APPEND LAMBDA (X Y)
    (IF (EQ X (QUOTE NIL)) Y
      (CONS (CAR X)
            (APPEND (CDR X) Y)))))

((A) (B C D))
(A B C D)

((A B C) (D))
(A B C D)

((A B C D)  (E F G H))
(A B C D E F G H)
