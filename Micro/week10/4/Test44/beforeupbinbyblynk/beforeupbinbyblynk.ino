#define BLYNK_TEMPLATE_ID "TMPL6_Dhyo6b3"
#define BLYNK_TEMPLATE_NAME "B6728793"
#define BLYNK_FIRMWARE_VERSION "1.1.0" // ตัวที่อยู่ในบอร์ดตอนนี้

#define BLYNK_PRINT Serial
#include "BlynkEdgent.h"

void setup() {
  Serial.begin(115200);
  // ระบบ Edgent จะจัดการ Wi-Fi และ OTA ของ Blynk ให้เองในนี้
  BlynkEdgent.begin(); 
}

void loop() {
  BlynkEdgent.run(); 
}