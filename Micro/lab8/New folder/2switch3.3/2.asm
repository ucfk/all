

ORG 0000H
    LJMP MAIN


ORG 0013H
    INC COUNT

  
    MOV A, COUNT
    CJNE A, #100, NEXT
    MOV COUNT, #00H

NEXT:
    
    ACALL DELAY_750MS


    CLR IE1

    RETI


MAIN:
    MOV P1, #00H
    MOV P2, #0FFH


    SETB IT1      
    SETB EX1   
    SETB EA        

    MOV COUNT, #27 

LOOP:


    MOV A, COUNT
    MOV B, #10
    DIV AB      

    MOV TEN, A
    MOV ONE, B


    CLR P2.1
    SETB P2.0

    MOV A, TEN
    MOV DPTR, #TABLE
    MOVC A, @A+DPTR
    MOV P1, A
    ACALL DELAY


    SETB P2.1
    CLR P2.0

    MOV A, ONE
    MOV DPTR, #TABLE
    MOVC A, @A+DPTR
    MOV P1, A
    ACALL DELAY

    SJMP LOOP

;-----------------------------
; Delay สั้น (multiplex)
;-----------------------------
DELAY:
    MOV R7, #5
D1: MOV R6, #200
D2: DJNZ R6, D2
    DJNZ R7, D1
    RET

;-----------------------------
; Delay ~750 ms
;-----------------------------
DELAY_750MS:
    MOV R5, #10
L1: MOV R6, #250
L2: MOV R7, #250
L3: DJNZ R7, L3
    DJNZ R6, L2
    DJNZ R5, L1
    RET

;-----------------------------
; Data
;-----------------------------
COUNT   DATA 30H
TEN     DATA 31H
ONE     DATA 32H

TABLE:
    DB 3FH,06H,5BH,4FH,66H,6DH,7DH,07H,7FH,6FH

END