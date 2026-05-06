#include <Arduino.h>

// ================== 1. Pin ==================
// Switch
#define swA 21
#define swB 19
#define swC 16
#define swD 17

// Pulse output
#define PULSE_OUT_PIN 4

// 7-seg segment pins (a b c d e f g)
int segPins[] = {2, 5, 18, 23, 22, 15, 13};

// digit pins D4 D3 D2 D1
int digitPins[] = {12, 14, 27, 26};

// ================== 2. Number table ==================
byte numbers[10][7] = {
  {1,1,1,1,1,1,0},
  {0,1,1,0,0,0,0},
  {1,1,0,1,1,0,1},
  {1,1,1,1,0,0,1},
  {0,1,1,0,0,1,1},
  {1,0,1,1,0,1,1},
  {1,0,1,1,1,1,1},
  {1,1,1,0,0,0,0},
  {1,1,1,1,1,1,1},
  {1,1,1,1,0,1,1}
};

// ================== 3. State ==================
volatile unsigned long pulseCount = 0;
volatile int displayValue = 0;

int pulseTarget   = 0;
int pulseInterval = 0;
bool running      = false;
unsigned long lastPulseTime = 0;

// ================== 4. Timer ==================
hw_timer_t *timer = NULL;
volatile int currentDigit = 0;

// ================== 5. Interrupt ==================
void IRAM_ATTR onTimer() {
  // ปิดทุก digit
  for (int i = 0; i < 4; i++) digitalWrite(digitPins[i], HIGH);

  int num = displayValue;

  int digits[4];
  digits[0] = (num / 1000) % 10;
  digits[1] = (num / 100) % 10;
  digits[2] = (num / 10) % 10;
  digits[3] = num % 10;

  // set segment
  for (int s = 0; s < 7; s++) {
    digitalWrite(segPins[s], numbers[digits[currentDigit]][s]);
  }

  // เปิด digit
  digitalWrite(digitPins[currentDigit], LOW);

  currentDigit++;
  if (currentDigit > 3) currentDigit = 0;
}

// ================== 6. Setup Pulse ==================
void setupPulse(int target, int interval) {
  pulseTarget   = target;
  pulseInterval = interval;
  pulseCount    = 0;
  displayValue  = 0;
  running       = true;
  lastPulseTime = millis();

  Serial.printf("Start: Target %d, Interval %d ms\n", target, interval);
}

// ================== 7. Setup ==================
void setup() {
  Serial.begin(115200);

  pinMode(swA, INPUT_PULLUP);
  pinMode(swB, INPUT_PULLUP);
  pinMode(swC, INPUT_PULLUP);
  pinMode(swD, INPUT_PULLUP);

  pinMode(PULSE_OUT_PIN, OUTPUT);
  digitalWrite(PULSE_OUT_PIN, LOW);

  // segment
  for (int i = 0; i < 7; i++) pinMode(segPins[i], OUTPUT);

  // digit
  for (int i = 0; i < 4; i++) pinMode(digitPins[i], OUTPUT);

  // Timer (1ms)
  timer = timerBegin(0, 80, true); // 1us tick
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);

  Serial.println("System Ready");
}

// ================== 8. Loop ==================
void loop() {
  // ---- ปุ่ม ----
  if (digitalRead(swA) == LOW) { setupPulse(100, 12); delay(300); }
  else if (digitalRead(swB) == LOW) { setupPulse(300, 6);  delay(300); }
  else if (digitalRead(swC) == LOW) { setupPulse(500, 3);  delay(300); }
  else if (digitalRead(swD) == LOW) { setupPulse(700, 1);  delay(300); }

  // ---- Pulse ----
  if (running) {
    if (pulseCount < (unsigned long)pulseTarget) {
      unsigned long now = millis();

      if (now - lastPulseTime >= (unsigned long)pulseInterval) {
        lastPulseTime = now;

        digitalWrite(PULSE_OUT_PIN, HIGH);
        delayMicroseconds(500);
        digitalWrite(PULSE_OUT_PIN, LOW);

        pulseCount++;

        // อัปเดตค่าให้ display (ไม่ต้อง render เองแล้ว)
        displayValue = pulseCount;
      }
    } else {
      running = false;
      Serial.println("Target Reached!");
    }
  }
}