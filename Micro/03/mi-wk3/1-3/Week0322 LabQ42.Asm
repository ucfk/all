;==================================================
; Q4.2: Open Table for Calc X^2+24
; Input   @8500H only 0-F
; Output  @8501H(HEX) and @8502H,8503H(BCD)
;==================================================
	ORG	0000H
	
Loop_Fill:	MOV	DPTR,#8500H
	MOVX	A,@DPTR	; Read Input
	MOV	DPTR,#Table_HEX
	MOVC	A,@A+DPTR
	MOV	DPTR,#8501H	; Save Input
	MOVX	@DPTR,A	
	
	MOV	DPTR,#8500H
	MOVX	A,@DPTR	; Read Input
	RL	A	; MUL 2
	PUSH	Acc	; Back_Up Data
	MOV	DPTR,#Table_BCD
	MOVC	A,@A+DPTR
	MOV	DPTR,#8502H	; Save Input
	MOVX	@DPTR,A	
	POP	Acc
	INC	A	; Mul2 + 1
	MOV	DPTR,#Table_BCD
	MOVC	A,@A+DPTR
	MOV	DPTR,#8503H	; Save Input
	MOVX	@DPTR,A	
	
	SJMP	$
	
Table_HEX:	DB	018H,019H,01CH,021H,028H,031H,03CH,049H
	DB	058H,069H,07CH,091H,0A8H,0C1H,0DCH,0F9H

Table_BCD:	DW	024H,025H,028H,033H,040H,049H,060H,073H
	DW	088H,105H,124H,145H,168H,193H,220H,249H

	
	END