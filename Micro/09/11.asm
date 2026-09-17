	ORG 0000H  
 	JMP 0100H 
 
 	ORG 0100H 
 	MOV SP,#3FH 
 	CLR EA 
 	MOV TMOD,#00010001B 

T_OFF: 	CLR TR1 		
	MOV A,#00H		
	MOV P2,A 		
	MOV DPTR,#-139+13 	; ???? OFF (25%) ?????? 3379 Hz
	MOV TH1,DPH 		
	MOV TL1,DPL 		
	CLR TF1 		
	SETB TR1 		
	JNB TF1,$ 		
	 			
T_ON: 	CLR TR1 		
	MOV A,#0FFH 		
	MOV P2,A 		
	MOV DPTR,#-419+15 	; ???? ON (75%) ?????? 3379 Hz
	MOV TH1,DPH 		
	MOV TL1,DPL 		
	CLR TF1 		
	SETB TR1 		
	JNB TF1,$ 		
	
	JMP T_OFF
	
	END