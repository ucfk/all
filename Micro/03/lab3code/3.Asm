ORG     8000H

START:
    MOV     DPTR, #TABLE    ; ชี้ไปที่จุดเริ่มต้นของข้อความ

MainLoop:
    CLR     A               
    MOVC    A, @A+DPTR      ; อ่านรหัสจาก Table
    
    CJNE    A, #0FFH, Show  ; ถ้าไม่ใช่ FFH ให้ไปแสดงผล
    SJMP    START           ; ถ้าเจอ FFH ให้กลับไปเริ่มที่ START (ตัวแรก)

Show:
    ; เก็บตำแหน่งปัจจุบันของข้อความไว้ใน Register
    MOV     R1, DPL         
    MOV     R2, DPH
    
    ; ส่งค่าออกไปยังหน้าจอ
    MOV     DPTR, #0E060H   
    MOVX    @DPTR, A        
    
    ACALL   DELAY           ; หน่วงเวลา
    
    ; ดึงตำแหน่งข้อความกลับมาเพื่อขยับไปตัวถัดไป
    MOV     DPL, R1         
    MOV     DPH, R2
    INC     DPTR            
    SJMP    MainLoop

;---------------------------------------
; ตารางรหัส 7-Segment (grade_Digital=A_A_)
;---------------------------------------
TABLE:
    DB  6FH ; g
    DB  50H ; r
    DB  77H ; a
    DB  5EH ; d
    DB  79H ; e
    DB  08H ; _
    DB  5EH ; D
    DB  06H ; i
    DB  6FH ; g
    DB  04H ; i
    DB  7cH ; t
    DB  77H ; a
    DB  38H ; l
    DB  48H ; =
    DB  77H ; A
    DB  08H ; (space)
    DB  77H ; A
    DB  08H ; (space)
    DB  0FFH ; 


DELAY: 	
    PUSH    07H 
    PUSH    06H 
    PUSH    05H 
    PUSH    04H 
    MOV     R4, #10         
_DLY_4: MOV     R5, #35
_DLY_5: MOV     R6, #17 
_DLY_6: MOV     R7, #76
_DLY_7: DJNZ    R7, _DLY_7 
    DJNZ    R6, _DLY_6 
    DJNZ    R5, _DLY_5 
    DJNZ    R4, _DLY_4 
    POP     04H 
    POP     05H 
    POP     06H 
    POP     07H 
    RET

    END