// กำหนดขา Pin
const int SW1 = 4;
const int LED1 = 22;
const int SW2 = 18;
const int LED2 = 23;

// ตัวแปรเก็บสถานะและเวลา
unsigned long startTime1 = 0;
unsigned long startTime2 = 0;
bool isPressing1 = false;
bool isPressing2 = false;

void setup() {
  pinMode(SW1, INPUT_PULLUP); // ใช้ Pull-up ภายใน (กดแล้วสถานะเป็น LOW)
  pinMode(LED1, OUTPUT);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(LED2, OUTPUT);
  
  digitalWrite(LED1, LOW); // เริ่มต้นปิดไฟ
  digitalWrite(LED2, LOW);
}

void loop() {
  // --- การจัดการ Switch 1 ---
  if (digitalRead(SW1) == LOW) { // เมื่อกดปุ่ม
    if (!isPressing1) {
      startTime1 = millis(); // บันทึกเวลาที่เริ่มกด
      isPressing1 = true;
    }
  } else { // เมื่อปล่อยปุ่ม
    if (isPressing1) {
      long duration = millis() - startTime1; // คำนวณระยะเวลาที่กดค้าง
      if (duration < 5000) { // กดไม่ถึง 5 วินาที (5000 ms)
        digitalWrite(LED1, HIGH); // เปิดไฟ
      } else {
        digitalWrite(LED1, LOW);  // ปิดไฟ
      }
      isPressing1 = false;
    }
  }

  // --- การจัดการ Switch 2 ---
  if (digitalRead(SW2) == LOW) {
    if (!isPressing2) {
      startTime2 = millis();
      isPressing2 = true;
    }
  } else {
    if (isPressing2) {
      long duration = millis() - startTime2;
      if (duration < 5000) {
        digitalWrite(LED2, HIGH);
      } else {
        digitalWrite(LED2, LOW);
      }
      isPressing2 = false;
    }
  }
}