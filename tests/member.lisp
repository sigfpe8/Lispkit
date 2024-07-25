(LETREC MEMBER
  (MEMBER LAMBDA (E L)
    (IF (EQ L (QUOTE NIL)) (QUOTE F)
    (IF (EQ E (CAR L)) (QUOTE T)
      (MEMBER E (CDR L))))))

(A (A B))
T

(B (A B))
T

(C (A B))
F
