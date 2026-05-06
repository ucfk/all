//esp boards manage version 2.0.17 onlyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
#define CUSTOM_SETTINGS 
#define INCLUDE_GAMEPAD_MODULE 
#include <DabbleESP32.h>
#define BLEDeviceName "B6728793_Kit" 

// กำหนดขา Pin สำหรับ LED 1-8 (ปรับเปลี่ยนเลขขาได้ตามความเหมาะสม)
const int ledPins[] = {2, 4, 5, 18, 19, 21, 22,23}; 
// สถานะของ LED แต่ละดวง (0 = Off, 1 = On)
int ledStates[] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() { 
  Serial.begin(115200); 
  Dabble.begin(BLEDeviceName); 
  
  // ตั้งค่าขา LED ทั้งหมดเป็น OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  Serial.println("Bluetooth Ready: " BLEDeviceName); 
} 

void loop() { 
  Dabble.processInput(); 

  // --- โซนปุ่มลูกศร (D-Pad) ---
  
  // LED 1: ลูกศรบน
  if (GamePad.isUpPressed()) { 
    toggleLED(0, "Up (LED 1)");
  } 
  // LED 2: ลูกศรซ้าย
  if (GamePad.isLeftPressed()) { 
    toggleLED(1, "Left (LED 2)");
  }
  // LED 3: ลูกศรขวา
  if (GamePad.isRightPressed()) { 
    toggleLED(2, "Right (LED 3)");
  }
  // LED 4: ลูกศรล่าง
  if (GamePad.isDownPressed()) { 
    toggleLED(3, "Down (LED 4)");
  }

  // --- โซนปุ่มสัญลักษณ์ (Shapes) ---

  // LED 5: สามเหลี่ยม
  if (GamePad.isTrianglePressed()) { 
    toggleLED(4, "Triangle (LED 5)");
  } 
  // LED 6: สี่เหลี่ยม
  if (GamePad.isSquarePressed()) { 
    toggleLED(5, "Square (LED 6)");
  }
  // LED 7: วงกลม
  if (GamePad.isCirclePressed()) { 
    toggleLED(6, "Circle (LED 7)");
  }
  // LED 8: กากบาท (Cross/X)
  if (GamePad.isCrossPressed()) { 
    toggleLED(7, "Cross (LED 8)");
  }
}

// ฟังก์ชันช่วยสลับสถานะไฟและป้องกันการกดค้าง (Debounce/Hold logic)
void toggleLED(int index, String label) {
  ledStates[index] = !ledStates[index]; // สลับสถานะ 0 <-> 1
  digitalWrite(ledPins[index], ledStates[index]);
  Serial.print(label);
  Serial.println(ledStates[index] ? " : ON" : " : OFF");

  // รอจนกว่าจะปล่อยปุ่ม เพื่อป้องกันไฟกระพริบถ้ารัวคำสั่ง
  while(true) {
    Dabble.processInput();
    if (index == 0 && !GamePad.isUpPressed()) break;
    if (index == 1 && !GamePad.isLeftPressed()) break;
    if (index == 2 && !GamePad.isRightPressed()) break;
    if (index == 3 && !GamePad.isDownPressed()) break;
    if (index == 4 && !GamePad.isTrianglePressed()) break;
    if (index == 5 && !GamePad.isSquarePressed()) break;
    if (index == 6 && !GamePad.isCirclePressed()) break;
    if (index == 7 && !GamePad.isCrossPressed()) break;
    delay(10); 
  }
}