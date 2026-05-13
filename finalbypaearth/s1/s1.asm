T2CON           EQU 0C8H
RCAP2L          EQU 0CAH 
RCAP2H          EQU 0CBH 

PortO_LEDGreen  EQU P3.7    

        ORG 0000H   
        JMP 0100H
    
        ORG 0100H
        MOV SP, #3FH
        CLR EA  
        CALL Delay_250M 
        CALL Initial_Serial 
        CALL Send_ClrScr    
        
        ; === 1. INITIALIZE TIMERS AS COUNTERS ===
; Set T0 and T1 pins as INPUTS before starting timers
        SETB P3.4       ; Make Pin P3.4 (T0) an input
        SETB P3.5       ; Make Pin P3.5 (T1) an input
        
        MOV TMOD, #55H  ; Set T1 and T0 to Mode 1 (16-bit Counter, C/T=1)
        SETB TR0        ; Start Counter 0
        SETB TR1        ; Start Counter 1

Main_Loop:  
        MOV DPTR,#Header_Fram       
        CALL Send_Table 
                
        ; === 2. READ COUNTER 0 (ESP D22) ===
        MOV R5, TL0         ; Read Low Byte of T0
        MOV R4, TH0         ; Read High Byte of T0
        
        ; === 3. READ COUNTER 1 (ESP D23) ===
        MOV R7, TL1         ; Read Low Byte of T1
        MOV R6, TH1         ; Read High Byte of T1

        ; === 4. DISPLAY R4R5 (T0) IN HEX ===
        MOV A, R4   
        CALL Send_2HEX
        MOV A, R5   
        CALL Send_2HEX
            
        ; Print Separator
        MOV A,#','
        CALL Send_ASCII
        
        ; === 5. DISPLAY R6R7 (T1) IN HEX ===
        MOV A, R6   
        CALL Send_2HEX
        MOV A, R7   
        CALL Send_2HEX
            
        CALL Send_LineFeed  
        CPL PortO_LEDGreen  
        
        CALL Delay_500M 
        
        JMP Main_Loop   

$INCLUDE (myDelay.A51)
$INCLUDE (mySerial.A51)
$INCLUDE (myHex2BCD.A51)  ; Kept in case you want to use DPTR2BCD later

Header_Fram:    DB  'Data Counter = ',00
        
        END