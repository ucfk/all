; กำหนดตำแหน่งตัวแปรในหน่วยความจำ RAM
DIG1        EQU 40H         ; หลักพัน (Thousands)
DIG2        EQU 41H         ; หลักร้อย (Hundreds)
DIG3        EQU 42H         ; หลักสิบ (Tens)
DIG4        EQU 43H         ; หลักหน่วย (Ones)

; กำหนดขาควบคุม 7-Segment
SEL1        EQU P2.0        ; เลือกหลักพัน
SEL2        EQU P2.1        ; เลือกหลักร้อย
SEL3        EQU P2.2        ; เลือกหลักสิบ
SEL4        EQU P2.3        ; เลือกหลักหน่วย
Segment_Port EQU P1         ; พอร์ตส่งข้อมูลไฟ LED (a-g)

        ORG    0000H 
        LJMP   START 

        ORG    0003H        ; เวกเตอร์ Interrupt 0 (EX0)
        LJMP   EX0_ROUTINE  ; ทำหน้าที่ +2

        ORG    0013H        ; เวกเตอร์ Interrupt 1 (EX1)
        LJMP   EX1_ROUTINE  ; ทำหน้าที่ -1

        ORG    0100H 
START:  
        ; ตั้งค่าการเกิด Interrupt (ทำงานเมื่อขอบขาลง - Falling Edge)
        SETB   IT0          
        SETB   IT1          
        
        ; เปิดการใช้งาน Global Interrupt และ EX0, EX1
        MOV    IE, #10000101B 
        
        ; กำหนดค่าเริ่มต้นเป็น 5670
        MOV    DIG1, #5
        MOV    DIG2, #6
        MOV    DIG3, #7
        MOV    DIG4, #0

Main_Loop: 
        CALL   Show_4Digits 
        SJMP   Main_Loop      


; =========================================
; Interrupt 0: เพิ่มค่าทีละ 2
; =========================================
EX0_ROUTINE: 
        PUSH   PSW
        PUSH   ACC

        ; หน่วงเวลาเพื่อกันสวิตช์กระเพื่อม (Debounce)
        MOV    R4, #20        
EX0_Deb_Loop:
        CALL   Show_4Digits 
        DJNZ   R4, EX0_Deb_Loop

        JB     P3.2, EXIT_EX0 ; ถ้าปุ่มถูกปล่อยแล้ว ให้ออก

        ; กระบวนการบวก 2
        INC    DIG4
        INC    DIG4

CHK_DIG4:
        MOV    A, DIG4
        CLR    C
        SUBB   A, #10
        JC     EX0_WAIT       ; ถ้าค่า < 10 ไม่ต้องทดข้ามหลัก
        MOV    DIG4, A        ; ถ้า >= 10 เก็บเศษไว้
        INC    DIG3           ; ทดไปหลักสิบ

CHK_DIG3:
        MOV    A, DIG3
        CLR    C
        SUBB   A, #10
        JC     EX0_WAIT
        MOV    DIG3, A
        INC    DIG2           ; ทดไปหลักร้อย

CHK_DIG2:
        MOV    A, DIG2
        CLR    C
        SUBB   A, #10
        JC     EX0_WAIT
        MOV    DIG2, A
        INC    DIG1           ; ทดไปหลักพัน

CHK_DIG1:
        MOV    A, DIG1
        CLR    C
        SUBB   A, #10
        JC     EX0_WAIT
        MOV    DIG1, A        ; ตัดกลับเป็น 0 ถ้าเกิน 9999

EX0_WAIT:
        ; หน่วงเวลาไม่ให้ตัวเลขวิ่งเร็วเกินไปหากกดค้าง
        MOV    R5, #25
EX0_Delay1: MOV R6, #5
EX0_Delay2: CALL Show_4Digits   
        DJNZ   R6, EX0_Delay2
        DJNZ   R5, EX0_Delay1
        
EXIT_EX0:
        CLR    IE0            ; เคลียร์ Flag
        POP    ACC
        POP    PSW
        RETI 


; =========================================
; Interrupt 1: ลดค่าทีละ 1
; =========================================
EX1_ROUTINE: 
        PUSH   PSW
        PUSH   ACC

        ; หน่วงเวลาเพื่อกันสวิตช์กระเพื่อม (Debounce)
        MOV    R4, #20        
EX1_Deb_Loop:
        CALL   Show_4Digits 
        DJNZ   R4, EX1_Deb_Loop

        JB     P3.3, EXIT_EX1 ; ถ้าปุ่มถูกปล่อยแล้ว ให้ออก

        ; กระบวนการลบ 1 (พร้อมเช็คการยืมค่า)
        MOV    A, DIG4
        JZ     BORROW_DIG4    ; ถ้าหลักหน่วยเป็น 0 ต้องยืม
        DEC    DIG4
        SJMP   EX1_WAIT

BORROW_DIG4:
        MOV    DIG4, #9       ; ถอยหลักหน่วยเป็น 9
        MOV    A, DIG3
        JZ     BORROW_DIG3    ; ถ้าหลักสิบเป็น 0 ต้องยืมต่อ
        DEC    DIG3
        SJMP   EX1_WAIT

BORROW_DIG3:
        MOV    DIG3, #9
        MOV    A, DIG2
        JZ     BORROW_DIG2
        DEC    DIG2
        SJMP   EX1_WAIT

BORROW_DIG2:
        MOV    DIG2, #9
        MOV    A, DIG1
        JZ     BORROW_DIG1
        DEC    DIG1
        SJMP   EX1_WAIT

BORROW_DIG1:
        MOV    DIG1, #9       ; ถ้า 0000 - 1 จะถอยกลับไป 9999

EX1_WAIT:
        ; หน่วงเวลาไม่ให้ตัวเลขวิ่งเร็วเกินไปหากกดค้าง
        MOV    R5, #25
EX1_Delay1: MOV R6, #5
EX1_Delay2: CALL Show_4Digits   
        DJNZ   R6, EX1_Delay2
        DJNZ   R5, EX1_Delay1
        
EXIT_EX1:
        CLR    IE1            ; เคลียร์ Flag
        POP    ACC
        POP    PSW
        RETI 


; =========================================
; ฟังก์ชันการสแกนแสดงผล 4 หลัก (Multiplexing)
; =========================================
Show_4Digits:
        MOV    DPTR, #CODE_TABLE
        
        ; แสดงหลักที่ 1 (พัน)
        MOV    A, DIG1
        MOVC   A, @A+DPTR
        MOV    Segment_Port, A
        CLR    SEL1           ; เปิดหลักที่ 1
        CALL   Scan_Delay     
        SETB   SEL1           ; ปิดหลักที่ 1
        
        ; แสดงหลักที่ 2 (ร้อย)
        MOV    A, DIG2
        MOVC   A, @A+DPTR
        MOV    Segment_Port, A
        CLR    SEL2           
        CALL   Scan_Delay     
        SETB   SEL2           

        ; แสดงหลักที่ 3 (สิบ)
        MOV    A, DIG3
        MOVC   A, @A+DPTR
        MOV    Segment_Port, A
        CLR    SEL3           
        CALL   Scan_Delay     
        SETB   SEL3           

        ; แสดงหลักที่ 4 (หน่วย)
        MOV    A, DIG4
        MOVC   A, @A+DPTR
        MOV    Segment_Port, A
        CLR    SEL4           
        CALL   Scan_Delay     
        SETB   SEL4           

        RET

; =========================================
; หน่วงเวลาสำหรับการสแกน LED
; =========================================
Scan_Delay:
        MOV    R7, #250
        DJNZ   R7, $
        RET

; ตารางรหัส 7-Segment (Common Cathode)
CODE_TABLE: 
        DB 3FH, 06H, 5BH, 4FH, 66H, 6D, 7DH, 07H, 7FH, 6FH

        END