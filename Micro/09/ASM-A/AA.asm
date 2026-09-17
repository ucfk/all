	ORG 0000H  
 	JMP 0100H 
 
 	ORG 0100H 
 	MOV SP,#3FH 
 	CLR EA 
 	MOV TMOD,#00010001B 
 
T_OFF: 	CLR TR1 		; Stop Timer 4 
 	MOV A,#00H		; Out Pulse 5 
 	MOV P2,A 		; Out P1 6 
 	MOV DPTR,#-615+13 	; Reload Data 8 
 	MOV TH1,DPH 		; For TH1 9 
 	MOV TL1,DPL 		; For TL1 10 
 	CLR TF1 		; Initial Start 11 
 	SETB TR1 		; Start Timer1 12 
 	JNB TF1,$ 		; Wait Until TF On 13 
 	  			; 14 
T_ON: 	CLR TR1 		; 1MC 15 
 	MOV A,#0FFH 		; 1MC 16 
 	MOV P2,A 		; 1MC 17 
 	MOV DPTR,#-315+15 	; 2MC 20 
 	MOV TH1,DPH 		; 2MC 21 
 	MOV TL1,DPL 		; 2MC 22 
 	CLR TF1 		; 1MC 23 
 	SETB TR1 		; 1MC 24 
 	JNB TF1,$ 		; 2MC 25 
 	 	 		; 26 
 	JMP T_OFF 		; 2MC 27 
 	 
 	END