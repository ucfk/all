ORG     0000H
        JMP     0100H
        ORG     0100H
        MOV     SP, #3FH
        CLR     EA
        SETB    P1.0
        JB      P1.0, $     
        MOV     TMOD, #21H
        MOV     SCON, #50H
        MOV     TH1, #0FDH
        SETB    TR1

        MOV     R7, #10 
LINE_LOOP:
        MOV     DPTR, #MY_INFO
        CALL    SEND_STRING  
        CALL    DELAY_500MS  
        DJNZ    R7, LINE_LOOP 

        
        MOV     DPTR, #WANT_A
        CALL    SEND_STRING  

STOP:   SJMP    STOP         

SEND_STRING:
        CLR     A
        MOVC    A, @A+DPTR
        JZ      STRING_DONE
        MOV     SBUF, A
        JNB     TI, $
        CLR     TI
        INC     DPTR
        SJMP    SEND_STRING
STRING_DONE:
        RET

DELAY_500MS:
        MOV     R2, #10      
DLY_500: CALL    DELAY
        DJNZ    R2, DLY_500
        RET

DELAY:  CLR     A
        MOV     B, A
DLY00:  DJNZ    ACC, DLY00
        DJNZ    B, DLY00
        RET

MY_INFO: DB      'B6728793 Kit Chanchuang',0DH,0AH,0 
WANT_A:  DB      'I Want A',0DH,0AH,0

        END