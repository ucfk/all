		ORG    	0000H

        	MOV     DPTR,#1000H
        	MOV    	A,#01
        	MOV    	R2,#01
        	MOV    	R3,#32
        	MOV    	R1,#40H

LOOP_SETVALUE:  MOVX	@DPTR,A
		MOVX	A,@DPTR
        	MOV    	@R1,A
        	ADD    	A,R2
        	INC     DPTR
        	INC    	R1
        	DJNZ    R3,LOOP_SETVALUE

        	JMP    	$
        	END