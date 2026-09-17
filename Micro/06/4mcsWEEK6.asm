ORG     0000H
        LJMP    MAIN
        ORG     0100H

MAIN:
        CLR     P1.7      
        MOV     R4, #00H  

MAIN_LOOP:
 
        JB      P1.3, EXECUTE_MODE 
        

        LCALL   DELAY_20MS  
        JB      P1.3, EXECUTE_MODE 
        

        MOV     R3, #50       
CHECK_HOLD:
        LCALL   DELAY_100MS
        JB      P1.3, SHORT_PRESS_ACTION
        DJNZ    R3, CHECK_HOLD

LONG_PRESS_ACTION:
        MOV     R4, #02H      
        SETB    P1.7         
        SJMP    WAIT_RELEASE 

SHORT_PRESS_ACTION:
        MOV     A, R4
        CJNE    A, #00H, TURN_OFF
        MOV     R4, #01H
        SJMP    WAIT_RELEASE

TURN_OFF:
        MOV     R4, #00H
        CLR     P1.7 

WAIT_RELEASE:
        JNB     P1.3, WAIT_RELEASE
        LCALL   DELAY_20MS
        SJMP    MAIN_LOOP


EXECUTE_MODE:
        MOV     A, R4
        CJNE    A, #01H, CHECK_MODE_2

        CPL     P1.7 
        LCALL   DELAY_100MS 
        SJMP    MAIN_LOOP

CHECK_MODE_2:
        CJNE    A, #02H, MODE_OFF

        SETB    P1.7
        SJMP    MAIN_LOOP

MODE_OFF:

        CLR     P1.7
        SJMP    MAIN_LOOP

DELAY_20MS:
        MOV     R0, #40
D_20:   MOV     R1, #250
        DJNZ    R1, $
        DJNZ    R0, D_20
        RET

DELAY_100MS:
        MOV     R0, #1
D1:     MOV     R1, #196
D2:     MOV     R2, #255
D3:     DJNZ    R2, D3
        DJNZ    R1, D2
        DJNZ    R0, D1
        RET

        END