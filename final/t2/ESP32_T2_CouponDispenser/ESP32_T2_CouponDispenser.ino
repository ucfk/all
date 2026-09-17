#include <TM1638plus.h>

#define pin_STB 18
#define pin_CLK 19
#define pin_DIO 21
#define S_Cancel 0 

TM1638plus tm(pin_STB, pin_CLK, pin_DIO, false);

const int price = 275;
const int values[8] = {1000, 500, 100, 50, 20, 10, 5, 1};
int credit = 0;

// --- ส่วนจัดการปุ่มแบบเสถียรพิเศษ ---
byte lastButtonState = 0; 
bool isButtonPressed = false; 
unsigned long lastDebounceTime = 0;  // เก็บเวลาที่ปุ่มเปลี่ยนสถานะล่าสุด
unsigned long debounceDelay = 80;    // ตั้งค่าหน่วงเวลา 80ms (ปรับเพิ่มได้ถ้ายังรัวอยู่)
bool lastCancelState = true;

void displayNumber(long number) {
  number = constrain(number, 0, 99999999);
  for (int pos = 7; pos >= 0; pos--) {
    tm.displayHex(pos, number % 10);
    number /= 10;
  }
}

void displayChange(int amount) {
  int tempAmount = amount;
  for (byte i = 0; i < 8; i++) {
    int count = tempAmount / values[i];
    tempAmount %= values[i];
    tm.displayHex(i, count % 10);
  }
}

void setAllLEDs(bool state) {
  for (byte i = 0; i < 8; i++) {
    tm.setLED(i, state ? 1 : 0);
  }
}

void setup() {
  pinMode(S_Cancel, INPUT_PULLUP);
  tm.displayBegin();
  tm.brightness(2);
  setAllLEDs(false);
  displayNumber(86728793); 
  delay(2500);
  displayNumber(credit);
}

void loop() {
  byte reading = tm.readButtons();

  // 1. ถ้าปุ่มมีการขยับ (ไม่ว่าจะกดหรือปล่อย) ให้เริ่มนับเวลาใหม่
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // 2. ถ้าเวลาผ่านไปนานกว่าค่า debounceDelay แสดงว่าสถานะปุ่ม "นิ่ง" แล้วจริงๆ
  if ((millis() - lastDebounceTime) > debounceDelay) {
    
    // จังหวะที่มีการกดจริง (และสถานะนิ่งแล้ว)
    if (reading != 0 && !isButtonPressed) {
      for (byte i = 0; i < 8; i++) {
        if (reading & (1 << i)) {
          credit += values[i];
          displayNumber(credit);
          isButtonPressed = true; // ล็อคทันที

          if (credit >= price) {
            int change = credit - price;
            setAllLEDs(true);
            displayChange(change);
            delay(4000);
            setAllLEDs(false);
            credit = 0;
            displayNumber(credit);
            isButtonPressed = false;
          }
          break;
        }
      }
    }

    // จังหวะที่ปล่อยปุ่มจนนิ่งแล้ว
    if (reading == 0) {
      isButtonPressed = false; 
    }
  }

  lastButtonState = reading;

  // ระบบ Cancel
  bool currentCancel = digitalRead(S_Cancel);
  if (lastCancelState == HIGH && currentCancel == LOW) {
    displayChange(credit);
    delay(4000);
    credit = 0;
    displayNumber(credit);
  }
  lastCancelState = currentCancel;
}