START:
    LDS  #$0FFF

    LDAA #1
    STAA $2000
    WAI
    STAA $2006

    JMP EOF

IRQH:
    STAA $2001
    LDAA #3
    STAA $2003
    RTI

SWIH:
    STAA $2001
    LDAA #2
    STAA $2002
    RTI

NMIH:

; -8 IRQ
    FDB IRQH
; -6 SWI
    FDB SWIH
; -4 NMI
    FDB NMIH
; -2 RESET
    FDB START

; emulator tester stops execution on file EOF
EOF:
