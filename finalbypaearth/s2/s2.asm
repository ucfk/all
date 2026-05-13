; =========================================================
; ??????????? MAX7219
DIN     BIT P1.0
CS      BIT P1.1
CLK     BIT P1.2
; ??????????? 7-Segment (Common Anode) ??????? P2
; =========================================================

        ORG 0000H
        LJMP MAIN

        ORG 0030H
MAIN:
        ; ??????? Timer 1 ?????? Serial (Baud 9600 @11.0592 MHz)
        MOV TMOD, #20H
        MOV TH1, #0FDH
        MOV SCON, #50H
        SETB TR1
        
        LCALL INIT_MAX7219
        MOV P2, #0FFH         ; ?????????? 7-Segment (??????)

WAIT_RX:
        JNB RI, WAIT_RX
        CLR RI
        MOV A, SBUF

        ; =================================================
        ; ????????????????? ASCII ???? Hex (0-15) ?????? SUBB
        ; =================================================
        MOV R1, A             ; ?????? A (ASCII) ????? R1

        ; ??????????? '0'-'9' (0x30 - 0x39) ????????
        CLR C
        SUBB A, #30H
        JC INVALID_RX         ; ??????????? '0' ???????

        MOV A, R1
        CLR C
        SUBB A, #3AH          ; '9' + 1
        JC IS_DIGIT           ; ??????????? 0x3A ??????????? 0-9

        ; ??????????? 'A'-'F' (0x41 - 0x46) ????????
        MOV A, R1
        CLR C
        SUBB A, #41H
        JC INVALID_RX         ; ?????????????? '9' ??? 'A' ???????

        MOV A, R1
        CLR C
        SUBB A, #47H          ; 'F' + 1
        JC IS_UPPER_HEX       ; ??????????? 0x47 ??????????? A-F

        ; ??????????? 'a'-'f' (0x61 - 0x66) ????????
        MOV A, R1
        CLR C
        SUBB A, #61H
        JC INVALID_RX         ; ?????????????? 'F' ??? 'a' ???????

        MOV A, R1
        CLR C
        SUBB A, #67H          ; 'f' + 1
        JC IS_LOWER_HEX       ; ??????????? 0x67 ??????????? a-f

        SJMP INVALID_RX       ; ?????????????????????? ???????

IS_DIGIT:
        MOV A, R1
        CLR C
        SUBB A, #30H          ; ???? '0'-'9' ???? 0-9
        SJMP PROCESS_DISPLAY

IS_UPPER_HEX:
        MOV A, R1
        CLR C
        SUBB A, #41H          ; ???? 'A'-'F' ???? 10-15
        ADD A, #0AH
        SJMP PROCESS_DISPLAY

IS_LOWER_HEX:
        MOV A, R1
        CLR C
        SUBB A, #61H          ; ???? 'a'-'f' ???? 10-15
        ADD A, #0AH

PROCESS_DISPLAY:
        MOV R0, A             ; ?????? Hex 0-15 ????? R0

        ; 1. ????????? 7-Segment
        MOV DPTR, #SEG_CA
        MOVC A, @A+DPTR
        MOV P2, A

        ; 2. ????????? Dot-matrix
        MOV A, R0
        LCALL UPDATE_MATRIX

INVALID_RX:
        SJMP WAIT_RX          ; ??????????????????????


; =========================================================
; ?????????????? MAX7219 (SPI Bit-Banging)
; =========================================================
INIT_MAX7219:
        MOV R2, #0BH          ; Scan Limit
        MOV R3, #07H
        LCALL SEND_SPI
        
        MOV R2, #09H          ; Decode Mode = 0
        MOV R3, #00H
        LCALL SEND_SPI
        
        MOV R2, #0AH          ; Intensity
        MOV R3, #07H
        LCALL SEND_SPI
        
        MOV R2, #0CH          ; Shutdown Reg = Normal
        MOV R3, #01H
        LCALL SEND_SPI
        RET

SEND_SPI:
        CLR CS
        MOV A, R2
        LCALL SHIFT_OUT
        MOV A, R3
        LCALL SHIFT_OUT
        SETB CS
        RET

SHIFT_OUT:
        MOV R4, #08H
SO_LOOP:
        RLC A
        MOV DIN, C
        SETB CLK
        CLR CLK
        DJNZ R4, SO_LOOP
        RET


; =========================================================
; ??????????????????? 8x8 ?? MAX7219
; =========================================================
UPDATE_MATRIX:
        MOV B, #08H
        MUL AB                ; A = Low byte (Offset), B = High byte
        
        MOV DPTR, #FONT_8X8
        MOV R4, A             ; ???? Low byte ????? R4
        MOV R5, B             ; ???? High byte ????? R5 (???????? Illegal Operand)
        
        ; ??? Offset ??????? Address ?????? DPTR
        MOV A, DPL
        ADD A, R4
        MOV DPL, A
        
        MOV A, DPH
        ADDC A, R5            ; ??? R5 ??? B ??????????????? 8051
        MOV DPH, A

        MOV R6, #01H          ; ??????????? Address 1 ??? MAX7219
UM_LOOP:
        MOV A, R6     ; ???? R6 ??????? A
	MOV R2, A     ; ??????? A ????? R2
        CLR A
        MOVC A, @A+DPTR
        MOV R3, A
        
        LCALL SEND_SPI        ; ????????????? MAX7219
        
        INC DPTR
        INC R6
        MOV A, R6
        CJNE A, #09H, UM_LOOP ; ?? 8 ???
        RET


; =========================================================
; ??????????? (Look-up Tables)
; =========================================================
SEG_CA:
        DB 0C0H, 0F9H, 0A4H, 0B0H, 099H, 092H, 082H, 0F8H ; 0 - 7
        DB 080H, 090H, 088H, 083H, 0C6H, 0A1H, 086H, 08EH ; 8 - F

FONT_8X8:
        DB 03CH, 042H, 042H, 042H, 042H, 042H, 042H, 03CH ; 0
        DB 008H, 018H, 028H, 008H, 008H, 008H, 008H, 03EH ; 1
        DB 03CH, 042H, 002H, 00CH, 010H, 020H, 040H, 07EH ; 2
        DB 03CH, 042H, 002H, 01CH, 002H, 002H, 042H, 03CH ; 3
        DB 00CH, 014H, 024H, 044H, 07EH, 004H, 004H, 004H ; 4
        DB 07EH, 040H, 040H, 07CH, 002H, 002H, 042H, 03CH ; 5
        DB 03CH, 040H, 040H, 07CH, 042H, 042H, 042H, 03CH ; 6
        DB 07EH, 002H, 004H, 008H, 010H, 020H, 020H, 020H ; 7
        DB 03CH, 042H, 042H, 03CH, 042H, 042H, 042H, 03CH ; 8
        DB 03CH, 042H, 042H, 042H, 03EH, 002H, 002H, 03CH ; 9
        DB 018H, 024H, 042H, 042H, 07EH, 042H, 042H, 042H ; A
        DB 07CH, 042H, 042H, 07CH, 042H, 042H, 042H, 07CH ; B
        DB 03CH, 042H, 040H, 040H, 040H, 040H, 042H, 03CH ; C
        DB 078H, 044H, 042H, 042H, 042H, 042H, 044H, 078H ; D
        DB 07EH, 040H, 040H, 07CH, 040H, 040H, 040H, 07EH ; E
        DB 07EH, 040H, 040H, 07CH, 040H, 040H, 040H, 040H ; F

        END