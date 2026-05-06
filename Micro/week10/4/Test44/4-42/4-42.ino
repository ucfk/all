#define BLYNK_TEMPLATE_ID "TMPL6_Dhyo6b3"
#define BLYNK_TEMPLATE_NAME "B6728793"
#define BLYNK_FIRMWARE_VERSION  "1.1.1"

#define BLYNK_PRINT Serial
#define APP_DEBUG

#include "BlynkEdgent.h"

#define LED_PIN 2

void setup()
{
  Serial.begin(115200);
  delay(100);

  pinMode(LED_PIN, OUTPUT);

  BlynkEdgent.begin();
}

void loop()
{
  BlynkEdgent.run();
  //digitalWrite(LED_PIN,HIGH); delay(500);
  //digitalWrite(LED_PIN,LOW); delay(500);

  digitalWrite(LED_PIN,HIGH); delay(250);
  digitalWrite(LED_PIN,LOW); delay(250);
  digitalWrite(LED_PIN,HIGH); delay(1000);
  digitalWrite(LED_PIN,LOW); delay(1000);
}