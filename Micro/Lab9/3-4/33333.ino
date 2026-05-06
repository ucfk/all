const int pulsePin = 23;

volatile unsigned long pulseCount = 0;
volatile bool newPulse = false;

void IRAM_ATTR onPulse() {
  pulseCount++;
  newPulse = true;
}
const int segPins[7] = {15,2,4,5,18,19,21};
const int digitPins[4] = {13,12,14,27};
const byte digits[10] = {
  0b0111111, // 0
  0b0000110, // 1
  0b1011011, // 2
  0b1001111, // 3
  0b1100110, // 4
  0b1101101, // 5
  0b1111101, // 6
  0b0000111, // 7
  0b1111111, // 8
  0b1101111  // 9
};

void showDigit(int pos, int num) {
  // ปิด digit ทั้งหมด
  for (int i = 0; i < 4; i++) digitalWrite(digitPins[i], HIGH); // CC: LOW=เปิด digit

  // ส่ง segment
  byte seg = digits[num];
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], (seg >> i) & 1);
  }

  // เปิด digit ที่ต้องการ
  digitalWrite(digitPins[pos], LOW);
  delayMicroseconds(3000); // 3ms per digit → refresh ~83Hz
}

void displayNumber(int num) {
  // จัดการค่าติดลบหรือเกิน 9999
  if (num < 0) num = (10000 + num % 10000) % 10000;
  if (num > 9999) num = num % 10000;

  int d[4];
  d[0] = (num / 1000) % 10; // หลักพัน
  d[1] = (num / 100)  % 10; // หลักร้อย
  d[2] = (num / 10)   % 10; // หลักสิบ
  d[3] =  num % 10; // หลักหน่วย

  for (int i = 0; i < 4; i++) {
    showDigit(i, d[i]);
  }
}

void setup() {
  pinMode(pulsePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pulsePin), onPulse, FALLING);
  Serial.begin(115200);
  Serial.println("RxBoard Ready - Waiting for pulses...");
  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
    digitalWrite(segPins[i], LOW);
  }
  // Digit pins
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH); // CC: HIGH = ปิด digit
  }
}

unsigned long lastPulseTime = 0;
unsigned long displayedCount = 0;
bool sessionActive = false;

void loop() {
  if (newPulse) {
    newPulse = false;
    lastPulseTime = millis();
    sessionActive = true;

    // แสดงตัวเลขนับขึ้น
    Serial.println(pulseCount);
  }

  // ถ้าหยุด pulse นานกว่า 500ms = จบชุดนี้แล้ว
  if (sessionActive && (millis() - lastPulseTime) > 500) {
    Serial.println("---- Done: " + String(pulseCount) + " pulses ----");
    Serial.println();
    pulseCount = 0;      // reset สำหรับรอบถัดไป
    sessionActive = false;
  }
  displayNumber(pulseCount);
}