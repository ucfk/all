#define Pin_RX2 16
#define Pin_TX2 17

// กำหนดขา GPIO สำหรับ LED 8 ดวง (ให้ตรงกับลำดับสวิตช์ของ Tx)
int ledPins[] = {4, 5, 18, 19, 21, 22, 23, 25}; 

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial2.setPins(Pin_RX2, Pin_TX2);

  // ตั้งค่าขา LED ทั้ง 8 เป็น Output และสั่งปิด LED เริ่มต้น
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); 
  }
}

void loop() {
  // ตรวจสอบว่ามีข้อมูลส่งมาจาก Tx หรือไม่
  if (Serial2.available() > 0) {
    int receivedData = Serial2.parseInt(); // อ่านค่าตัวเลขที่ส่งมา

    //parseInt() อาจจะอ่านเจอตัวอักษรว่างๆ (0) จึงให้ทำงานเฉพาะเมื่อมีค่าที่ถูกต้องหรือมีการเปลี่ยนแปลง
    Serial.print("Rx Received: ");
    Serial.println(receivedData); // แสดงค่าที่รับได้บน Serial Monitor

    // วนลูปแยกบิตเพื่อสั่งงาน LED ทีละดวง
    for (int i = 0; i < 8; i++) {
      // เช็คว่าบิตที่ i เป็น 1 หรือ 0
      if ((receivedData >> i) & 0x01) {
        digitalWrite(ledPins[i], HIGH); // ถ้าบิตเป็น 1 สั่ง LED ติด (Active High)
      } else {
        digitalWrite(ledPins[i], LOW);  // ถ้าบิตเป็น 0 สั่ง LED ดับ
      }
    }
  }
}