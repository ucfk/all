Data_Count    EQU 40H         ; Variable to store number
Left_Select   EQU P1.1        ; Left digit control
Right_Select  EQU P1.2        ; Right digit control
Segment_Port  EQU P2          ; 7-segment data port

        ORG    0000H 
        LJMP   START   
        ORG    0013H          
        LJMP   EX1_ROUTINE 
        ORG    0100H 
START:  
        SETB   IT1            ; Set INT1 as falling edge trigger
        MOV    IE, #10000100B ; Enable global interrupt and INT1
        MOV    Data_Count, #93 ; Initial value = 29

Main_Loop: 
        CALL   Show_2Digits   ; Display number
        SJMP   Main_Loop      
EX1_ROUTINE: 
        PUSH   ACC            ; Save ACC
        PUSH   B              ; Save B
        CLR    EX1            ; Disable INT1 to prevent bouncing
        MOV    R4, #50        
        
Deb_Loop:
        CALL   Show_2Digits   ; Keep display running
        DJNZ   R4, Deb_Loop

        ; ---------- Check if button still pressed ----------
        JB     P3.3, EXIT_ISR ; If released, exit ISR

        ; ---------- Increment Counter ----------
        INC    Data_Count
        
        MOV    A, Data_Count
        CJNE   A, #100, Skip_Reset
        MOV    Data_Count, #0 ; Reset if reaches 100
Skip_Reset:

        ; ---------- Post-delay ----------
        MOV    R5, #100       
Wait_Loop:
        CALL   Show_2Digits
        DJNZ   R5, Wait_Loop

EXIT_ISR:
        CLR    IE1            ; Clear interrupt flag
        SETB   EX1            ; Re-enable INT1

        POP    B              ; Restore B
        POP    ACC            ; Restore ACC
        RETI                  ; Return from interrupt

;==========================================================
; Display 2 Digits (Multiplexing)
;==========================================================
Show_2Digits:
        MOV    A, Data_Count
        MOV    B, #10
        DIV    AB             ; A = tens, B = units
        
        ; ----- Display Tens Digit -----
        PUSH   B              
        MOV    DPTR, #CODE_TABLE
        MOVC   A, @A+DPTR
        MOV    Segment_Port, A
        CLR    Left_Select    ; Enable left digit
        SETB   Right_Select   ; Disable right digit
        CALL   Scan_Delay     
        SETB   Left_Select    

        ; ----- Display Units Digit -----
        POP    ACC            
        MOVC   A, @A+DPTR
        MOV    Segment_Port, A
        CLR    Right_Select   ; Enable right digit
        SETB   Left_Select    ; Disable left digit
        CALL   Scan_Delay     
        SETB   Right_Select   
        RET

;==========================================================
; Small Delay for Multiplexing
;==========================================================
Scan_Delay:
        MOV    R7, #250
        DJNZ   R7, $
        RET

;==========================================================
; 7-Segment Code Table (Common Cathode)
;==========================================================
CODE_TABLE: 
        DB 3FH, 06H, 5BH, 4FH, 66H
        DB 6DH, 7DH, 07H, 7FH, 6FH

        END