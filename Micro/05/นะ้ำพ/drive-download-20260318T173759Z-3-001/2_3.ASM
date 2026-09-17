	ORG	0000H
	
	MOV	DPTR , #0000H
	
	MOV	B , DPH
	MOV	A , B
	CPL	A
	MOV	B , A
	
	MOV	A , #00
	CLR	C
	
	MOV	C,B.7
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.6
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.5
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.4
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.3
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.2
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.1
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.0
	ADDC 	A , #00
	DA	A
	
	
	MOV	R0 , A
	MOV	B , DPL
	MOV	A , B
	CPL	A
	MOV	B , A
	
	MOV	A , R0
	
	MOV	C,B.7
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.6
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.5
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.4
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.3
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.2
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.1
	ADDC 	A , #00
	DA	A
	
	MOV	C,B.0
	ADDC 	A , #00
	DA	A
	
	MOV	B,A
	
	JMP	$
	END