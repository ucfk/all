;==================================================
; Q4.3: Time Delay Calculation
;==================================================
	ORG	8000H
	MOV	DPTR,#0E060H
	
Loop_Fill:	MOV	A,#00111111B	; TGFE DCBA Show-0
	MOVX	@DPTR,A
	CALL	Delay_1Sec
	
	MOV	A,#00000110B	; TGFE DCBA Show-1
	MOVX	@DPTR,A
	CALL	Delay_1Sec
	
	JMP	Loop_Fill
	
;#=========================================================================
;#*        Strat Display  for XTAL 11.0592 MHz 12CLK/MC
;#=========================================================================
DELAY_4SEC:	CALL	Delay_1Sec
	CALL	Delay_1Sec
	CALL	Delay_1Sec
	CALL	Delay_1Sec
	RET

Delay_2Sec:	CALL	Delay_1Sec
	CALL	Delay_1Sec
	RET
	
DELAY_1SEC:	CALL	Delay_250M
	CALL	Delay_250M
	CALL	Delay_250M
	CALL	Delay_250M
	RET

DELAY_250M:	PUSH	07H
	PUSH	06H
	PUSH	05H
	MOV	R7,#5	; x Delay 250 MilliSec
__DELAY_50M:	MOV	R6,#200	; y Delay 50 MilliSec
_DELAY_250U:	MOV	R5,#118	; z Delay 250 mSec  
	DJNZ	R5,$	; x*1.059uSec*2MC = 250 uSec
	DJNZ	R6,_DELAY_250U 	; y*250uSec 	= 50  mSec
	DJNZ	R7,__DELAY_50M    ; z*50ms          = 250 mSec
	POP	05H
	POP	06H
	POP	07H
	RET
	
		
	END