ORG     0000H
        JMP     0100H

        ORG     0100H
        SETB    P1.3
        SETB    P1.7

LOOP:   JB 	P1.3,$
	JNB     P1.3, HOLD
        

HOLD:   
	JNB	P1.3,$
	SETB    P1.7
        JNB      P1.3, SW
        JMP	HOLD

SW:     
        JNB	P1.3, $
        CPL     P1.7
        CALL    DELAY
        JB	P1.3, SW
        JMP    	LOOP

DELAY:  MOV     00H, #10
D1:     MOV     01H, #100
D2:     MOV     B,   #100
D3:     DJNZ    B,   D3
        DJNZ    01H, D2
        DJNZ    00H, D1
        RET

        END