START:
    LDAA #%10100000
    STAA $4002
    LDAA #%00000111
    STAA $4003
    LDAA #255
    STAA $4002
    LDAA $4002
    STAA $2000
    JMP EOF

IRQH:

SWIH:

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
