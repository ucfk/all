ORG 0000H
        SJMP MAIN

        ORG 0030H
MAIN:
        MOV TMOD, #20H   ; ตั้งค่า Timer 1 เป็น Mode 2 (8-bit auto-reload)
        MOV TH1, #0FDH   ; ตั้งค่า Baud Rate = 9600 (สำหรับคริสตัล 11.0592 MHz)
        MOV SCON, #50H   ; Serial Mode 1, เปิดใช้งานการรับส่ง (REN=1)
        SETB TR1         ; เริ่มการทำงาน Timer 1

CHECK_SW:
        JB P1.0, CHK_SW1 ; ตรวจสอบ SW0 (ถ้าไม่ได้กด ให้ไปเช็ค SW1)
        MOV A, #'0'      ; ใส่รหัส ASCII ของเลข 0 (30H) ลงใน A
        ACALL SEND_SER
        ACALL DELAY      ; หน่วงเวลาเพื่อกันการกดปุ่มซ้ำ (Debounce)
        SJMP CHECK_SW

CHK_SW1:
        JB P1.1, CHK_SW2 ; ตรวจสอบ SW1
        MOV A, #'1'      ; ใส่รหัส ASCII ของเลข 1 (31H) ลงใน A
        ACALL SEND_SER
        ACALL DELAY
        SJMP CHECK_SW

CHK_SW2:
        JB P1.2, CHK_SW3 ; ตรวจสอบ SW2
        MOV A, #'2'      ; ใส่รหัส ASCII ของเลข 2 (32H) ลงใน A
        ACALL SEND_SER
        ACALL DELAY
        SJMP CHECK_SW

CHK_SW3:
        JB P1.3, CHECK_SW ; ตรวจสอบ SW3 (ถ้าไม่ได้กด ให้วนกลับไปเช็ค SW0 ใหม่)
        MOV A, #'3'       ; ใส่รหัส ASCII ของเลข 3 (33H) ลงใน A
        ACALL SEND_SER
        ACALL DELAY
        SJMP CHECK_SW

SEND_SER:
        MOV SBUF, A      ; ส่งข้อมูลออกจากพอร์ต Serial
WAIT_TX:
        JNB TI, WAIT_TX  ; รอจนกว่าจะส่งข้อมูลเสร็จ (TI=1)
        CLR TI           ; เคลียร์แฟล็ก TI เพื่อเตรียมส่งครั้งต่อไป
        RET

DELAY:                   ; โปรแกรมย่อยสำหรับหน่วงเวลา
        MOV R2, #255
DL1:    MOV R3, #255
DL2:    DJNZ R3, DL2
        DJNZ R2, DL1
        RET
        END