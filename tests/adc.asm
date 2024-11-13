    LDAA #10     ; Load 10 into A
    DECA         ; Decrement A (A = 9)
    ADCA #1      ; Add 1 to A with carry
    STAA $2000   ; Store result (10) in $2000
