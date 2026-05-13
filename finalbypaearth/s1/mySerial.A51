;============  Initial Serial Port (11.0592 MHz Crystal) ============
Initial_Serial: 
        ; Using explicit hex addresses to bypass "Undefined" compiler errors
        MOV     0C8H, #30H          ; T2CON (0C8H): Setup Timer 2
        MOV     0CBH, #0FFH         ; RCAP2H (0CBH): High byte for 9600 bps
        MOV     0CAH, #0DCH         ; RCAP2L (0CAH): Low byte for 9600 bps
        
        MOV     0CDH, #0FFH         ; TH2 (0CDH): Pre-load High (Fix for Line 6)
        MOV     0CCH, #0DCH         ; TL2 (0CCH): Pre-load Low  (Fix for Line 7)
        
        MOV     SCON, #50H          ; Serial Mode 1, REN=1
        MOV     0C8H, #34H          ; T2CON (0C8H): Start Timer 2 (Fix for Line 9)
        CLR     EA                  ; Disable All Interrupts
        RET
			
;==========  Send Data to Serial Port   ==========
SEND_2HEX:	PUSH	Acc	
		SWAP	A	
		CALL	SEND_1HEX	
		POP	Acc	
SEND_1HEX:	PUSH	DPH	
		PUSH	DPL	
		MOV	DPTR,#HEXASC_TAB	
		ANL	A,#0FH	
		MOVC	A,@A+DPTR	
		POP	DPL	
		POP	DPH	
SEND_ASCII:	PUSH	IE	
		CLR	ES	
		CLR    	TI	
		MOV    	SBUF,A	
		JNB    	TI,$	
		CLR    	TI	
		POP	IE	
		RET		
HEXASC_TAB: 	DB	'0123456789ABCDEF'	
			
;=========   Send Line Feed =================
Send_LineFeed:	MOV	A,#0DH	
		CALL	Send_ASCII	
		MOV	A,#0AH	
		CALL	Send_ASCII	
		RET		
			
;========  Send Line Feed ==================
Send_ClrScr:	MOV	A,#0CH	
		CALL	Send_ASCII	
		RET		
			
;== Send Data Table to Serial Start@DPTR to '00H' =
Send_Table:	CLR	A	
		MOVC	A,@A+DPTR	
		JZ	_X_Send_Tab	
		CALL	SEND_ASCII	
		INC	DPTR	
		JMP	Send_Table	
_X_Send_Tab:	RET		
			
;========= Send 1-Byte@Acc to SERIAL =========
TX_BYTE:	PUSH	IE	
		CLR	ES	
		CLR	TI	
		MOV	SBUF,A	
		JNB	TI,$	
		CLR	TI	
		POP	IE	
		RET		
			
;====== Receive Data From SERIAL to Acc ========
RX_BYTE:	PUSH	IE	
		CLR	ES	
		JNB	RI,$	; Wait data
		CLR	RI	
		MOV	A,SBUF	
		POP	IE	
		RET 		
