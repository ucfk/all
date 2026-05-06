	ORG 	8000H
	MOV 	DPTR,#0E060H
Loop: 	MOV 	A,#7CH ; Show B
	CALL 	Disp7Seg
	MOV 	A,#7DH ; Show 6
	CALL 	Disp7Seg
	MOV 	A,#6DH ; Show 5
	CALL 	Disp7Seg
	MOV 	A,#06H ; Show 1
	CALL 	Disp7Seg
	MOV 	A,#6DH ; Show 5
	CALL 	Disp7Seg
	MOV 	A,#7DH ; Show 6
	CALL 	Disp7Seg
	MOV 	A,#6DH ; Show 5
	CALL 	Disp7Seg
	MOV 	A,#5BH ; Show 2
	CALL 	Disp7Seg
	JMP 	Loop
Disp7Seg:
	MOVX 	@DPTR,A
	
DELAY: 	PUSH 07H 
	PUSH 06H 
	PUSH 05H 
	PUSH 04H 
	MOV R4,#10 
_DLY_4: MOV R5,#15 
_DLY_5: MOV R6,#30 
_DLY_6: MOV R7,#27 
_DLY_7: DJNZ R7,_DLY_7 
	DJNZ R6,_DLY_6 
	DJNZ R5,_DLY_5 
	DJNZ R4,_DLY_4 
	POP 04H 
	POP 05H 
	POP 06H 
	POP 07H 
	RET
	
	END