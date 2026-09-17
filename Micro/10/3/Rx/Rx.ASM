ORG 0000H
        SJMP MAIN

        ORG 0030H
MAIN:
        MOV TMOD, #20H   ;  Mode 2
        MOV TH1, #0FDH   ; Baud Rate = 9600
        MOV SCON, #50H   ;
        SETB TR1         ;Timer 1

WAIT_RX:
        JNB RI, WAIT_RX  ; (RI=1)
        CLR RI          
        MOV A, SBUF      

     
        CJNE A, #'0', CHK_GE_0
CHK_GE_0:
        JC WAIT_RX       
        
        CJNE A, #'9'+1, CHK_LE_9
CHK_LE_9:
        JNC WAIT_RX      

        ;  Index 0-9
        CLR C
        SUBB A, #30H     

        ;  7-Segment
        MOV DPTR, #SEG_DATA
        MOVC A, @A+DPTR  
        MOV P2, A        
        SJMP WAIT_RX     
SEG_DATA:
        ; ตารางข้อมูล 7-Segment แบบ Common Anode สำหรับเลข 0 - 9
        DB 0C0H, 0F9H, 0A4H, 0B0H, 099H, 092H, 082H, 0F8H, 080H, 090H
        
        END