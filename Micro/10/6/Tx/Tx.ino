#define Pin_RX2 16
#define Pin_TX2 17

// กำหนดขา GPIO สำหรับสวิตช์ 8 ตัว (แก้ตามที่ต่อจริงได้เลย)
int swPins[] = {4, 5, 18, 19, 21, 22, 23, 25}; 

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial2.setPins(Pin_RX2, Pin_TX2);

  // ตั้งค่าขา Switch ทั้ง 8 เป็น Input (ใช้ PULLUP ป้องกันสัญญาณรบกวน)
  for (int i = 0; i < 8; i++) {
    pinMode(swPins[i], INPUT_PULLUP);
  }
}

void loop() {
  int dataToSend = 0; // ตัวแปรเก็บค่ารวมของสวิตช์ทั้ง 8 (ค่า 0 - 255)

  // วนลูปอ่านค่าสวิตช์ทั้ง 8 ตัว
  for (int i = 0; i < 8; i++) {
    // ถ้าสวิตช์ถูกกด (สมมติว่าเป็น Active Low คือกดแล้วเป็น LOW)
    // **หมายเหตุ:** ถ้าบอร์ดสวิตช์ของคุณกดแล้วเป็น HIGH ให้เปลี่ยน == LOW เป็น == HIGH
    if (digitalRead(swPins[i]) == LOW) { 
      dataToSend |= (1 << i);  // นำบิตที่ i ไปเซตค่าเป็น 1 ใน dataToSend
    }
  }

  Serial.print("Tx Sending: ");
  Serial.println(dataToSend);   // แสดงค่าที่ส่งบน Serial Monitor เครื่องตัวเอง
  
  Serial2.println(dataToSend);  // ส่งข้อมูลตัวเลขไปยัง PC-คี่ (Rx)
  
  delay(100); // หน่วงเวลาส่งข้อมูลเล็กน้อย
}
