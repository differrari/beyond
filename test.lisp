((label fib (lambda (n) 
    (cond
        ((eq n 0) 0)
        ((eq n 1) 0)
        ((eq n 2) 1)
        (t (add (fib (sub n 1)) (fib (sub n 2))))
))) 6)
//;Might not wanna get this higher than 10, it's not optimized