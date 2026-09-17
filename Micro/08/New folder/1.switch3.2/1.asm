;=============================
; LAB: 7SEG oo Blink + Interrupt
;=============================

FLAG    BIT 20H   
DIGIT   BIT 21H   
BLINK   BIT 22H   

COUNT   DATA 30H  

ORG 0000H
    LJMP MAIN


ORG 0003H
    CPL FLAG
    RETI


ORG 000BH


    MOV TH0, #0FCH
    MOV TL0, #066H


    INC COUNT
    MOV A, COUNT
    CJNE A, #200, NEXT 

    MOV COUNT, #00H
    CPL BLINK           

NEXT:


    CPL DIGIT

 
    SETB P2.0
    SETB P2.1

 
    JNB BLINK, OFF

 
    JB FLAG, BOT

TOP:
    MOV A, #063H
    SJMP SHOW

BOT:
    MOV A, #05CH

SHOW:
    MOV P1, A


    JB DIGIT, D2

D1:
    CLR P2.0
    SJMP EXIT

D2:
    CLR P2.1
    SJMP EXIT

OFF:
    MOV P1, #00H   

EXIT:
    RETI


MAIN:
    MOV P1, #00H
    MOV P2, #0FFH

  
    MOV TMOD, #01H
    MOV TH0, #0FCH
    MOV TL0, #066H
    SETB ET0
    SETB TR0


    SETB IT0
    SETB EX0
    SETB EA

    CLR FLAG
    CLR DIGIT
    SETB BLINK

    MOV COUNT, #00H

LOOP:
    SJMP LOOP

END