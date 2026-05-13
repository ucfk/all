#define ROT_A 32
#define ROT_B 33
#define ROT_SW 25

/*
  U3: ESP32 อ่าน Rotary Encoder แล้วแสดงผลบน LED 8 ดวง

  การต่อวงจร:
  - Rotary encoder A/B = GPIO32/GPIO33
  - ปุ่มกดใน rotary = GPIO25 แบบ active LOW ใช้รีเซ็ตค่าเป็น 0
  - LED0..LED7 = GPIO2,4,5,18,19,21,22,23

  การทำงาน:
  - ค่า 0 ติด LED0
  - ค่า 1 ติด LED0 และ LED1
  - ...
  - ค่า 7 ติด LED0..LED7
  - ค่าอื่นนอกช่วง 0..7 ทำให้ LED ทั้งหมดกระพริบประมาณ 1 Hz

  จุดที่แก้ได้:
  - ledPins[] และ ROT_* ถ้าต่อสายไม่ตรงกับโค้ด
*/

const byte ledPins[8] = {2, 4, 5, 18, 19, 21, 22, 23};

volatile int position = 0;
volatile int lastEncoded = 0;
unsigned long lastBlink = 0;
bool blinkState = false;

void IRAM_ATTR readEncoder() {
  // ถอดรหัส quadrature ใน interrupt โดยอัปเดตเฉพาะค่า position งานแสดง LED อยู่ใน loop()
  int msb = digitalRead(ROT_A);
  int lsb = digitalRead(ROT_B);
  int encoded = (msb << 1) | lsb;
  int sum = (lastEncoded << 2) | encoded;
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) position++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) position--;
  lastEncoded = encoded;
}

void showBar(int value) {
  // ช่วงปกติแสดงแบบ bar graph ถ้านอกช่วงให้ LED ทั้งหมดกระพริบ
  if (value >= 0 && value <= 7) {
    for (byte i = 0; i < 8; i++) digitalWrite(ledPins[i], i <= value);
    return;
  }
  if (millis() - lastBlink >= 500) {
    lastBlink = millis();
    blinkState = !blinkState;
    for (byte i = 0; i < 8; i++) digitalWrite(ledPins[i], blinkState);
  }
}

void setup() {
  pinMode(ROT_A, INPUT_PULLUP);
  pinMode(ROT_B, INPUT_PULLUP);
  pinMode(ROT_SW, INPUT_PULLUP);
  for (byte i = 0; i < 8; i++) pinMode(ledPins[i], OUTPUT);
  lastEncoded = (digitalRead(ROT_A) << 1) | digitalRead(ROT_B);
  attachInterrupt(digitalPinToInterrupt(ROT_A), readEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROT_B), readEncoder, CHANGE);
}

void loop() {
  if (digitalRead(ROT_SW) == LOW) position = 0;

  noInterrupts();
  int currentPosition = position;
  interrupts();

  showBar(currentPosition);
}
