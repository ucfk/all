;==================================================
; Q4.1: Fill 55 to Data 9000 to 9200H
;==================================================
	ORG	0000H
	MOV	B,#55H	; Data Fill
	MOV	DPTR,#9000H
	
Loop_Fill:	MOV	A,B	; ReLoad Data
	MOVX	@DPTR,A
	INC	DPTR
	MOV	A,DPH
	CJNE	A,#92H,Loop_Fill
	MOV	A,DPL
	CJNE	A,#01H,Loop_Fill
	
	SJMP	$
	
	END