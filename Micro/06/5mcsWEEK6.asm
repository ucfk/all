ORG     0000H
        LJMP    MAIN
        ORG     0100H

MAIN:
        SETB    P1.7 
        MOV     R4, #0 

MAIN_LOOP:

        JB      P1.3, EXECUTE_MODE

        LCALL   DELAY_20MS 
        JB      P1.3, EXECUTE_MODE
        
        INC     R4   
        

        MOV     A, R4
        CJNE    A, #3, WAIT_REL 
        MOV     R4, #1        

WAIT_REL:
        JNB     P1.3, WAIT_REL 
        LCALL   DELAY_20MS 


EXECUTE_MODE:
        MOV     A, R4

        CJNE    A, #2, LED_ON_STAY 

        SJMP    MAIN_LOOP

LED_ON_STAY:
        SETB    P1.7  
        SJMP    MAIN_LOOP

DELAY_20MS:
        MOV     R0, #40
D_20:   MOV     R1, #250
        DJNZ    R1, $
        DJNZ    R0, D_20
        RET

DELAY_100MS:
        MOV     R1, #200
D1:     MOV     R2, #250
        DJNZ    R2, $
        DJNZ    R1, D1
        RET

        END