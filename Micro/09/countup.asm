CNT_LO  EQU     30h
CNT_HI  EQU     31h
D0      EQU     32h
D1      EQU     33h
D2      EQU     34h
D3      EQU     35h
DB0     EQU     36h
DB1     EQU     37h

        ORG     0000h
        LJMP    MAIN
        ORG     0003h
        LJMP    ISR_INT0
        ORG     0013h
        LJMP    ISR_INT1

        ORG     0100h
SEG_TABLE:
        DB      03Fh,006h,05Bh,04Fh,066h,06Dh,07Dh,007h,07Fh,06Fh

DIGIT_MASK:
        DB      0FEh,0FDh,0FBh,0F7h

        ORG     0120h
MAIN:
        MOV     SP,  #70h
        MOV     P1,  #00h
        MOV     P2,  #0FFh
        MOV     CNT_LO, #0F3h
        MOV     CNT_HI, #00Eh
        MOV     DB0, #00h
        MOV     DB1, #00h
        SETB    IT0
        SETB    IT1
        SETB    EX0
        SETB    EX1
        SETB    EA

LOOP:
        LCALL   SPLIT_DIGITS
        LCALL   SHOW_ALL
        SJMP    LOOP

SPLIT_DIGITS:
        MOV     R6, CNT_LO
        MOV     R7, CNT_HI
        LCALL   DIV16_10
        MOV     D3, A
        LCALL   DIV16_10
        MOV     D2, A
        LCALL   DIV16_10
        MOV     D1, A
        LCALL   DIV16_10
        MOV     D0, A
        RET

DIV16_10:
        MOV     38h, #00h
        MOV     39h, #00h
DT_LOOP:
        MOV     A, R7
        JNZ     DT_DO_SUB
        CLR     C
        MOV     A, R6
        SUBB    A, #0Ah
        JC      DT_DONE
DT_DO_SUB:
        CLR     C
        MOV     A, R6
        SUBB    A, #0Ah
        MOV     R6, A
        MOV     A, R7
        SUBB    A, #00h
        MOV     R7, A
        MOV     A, 38h
        ADD     A, #01h
        MOV     38h, A
        MOV     A, 39h
        ADDC    A, #00h
        MOV     39h, A
        SJMP    DT_LOOP
DT_DONE:
        MOV     A,  R6
        MOV     R6, 38h
        MOV     R7, 39h
        RET

SHOW_ALL:
        MOV     R0, #D0
        MOV     R3, #00h
SA_LOOP:
        MOV     A, @R0
        LCALL   SHOW_DIGIT
        INC     R0
        INC     R3
        CJNE    R3, #04h, SA_LOOP
        RET

SHOW_DIGIT:
        MOV     P2, #0FFh
        MOV     DPTR, #SEG_TABLE
        MOVC    A, @A+DPTR
        MOV     P1, A
        MOV     A, R3
        MOV     DPTR, #DIGIT_MASK
        MOVC    A, @A+DPTR
        MOV     P2, A
        MOV     R1, #08h
DLY_O:  MOV     R2, #0FFh
DLY_I:  DJNZ    R2, DLY_I
        DJNZ    R1, DLY_O
        MOV     P2, #0FFh
        RET

ISR_INT0:
        PUSH    ACC
        PUSH    PSW
        CLR     C
        MOV     A, CNT_LO
        ADD     A, #02h
        MOV     CNT_LO, A
        MOV     A, CNT_HI
        ADDC    A, #00h
        MOV     CNT_HI, A
        POP     PSW
        POP     ACC
        RETI

ISR_INT1:
        PUSH    ACC
        PUSH    PSW
        CLR     C
        MOV     A, CNT_LO
        SUBB    A, #01h
        MOV     CNT_LO, A
        MOV     A, CNT_HI
        SUBB    A, #00h
        MOV     CNT_HI, A
        POP     PSW
        POP     ACC
        RETI

        END