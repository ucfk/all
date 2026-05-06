#define BLYNK_TEMPLATE_ID "TMPL6EMlD0dlk"
#define BLYNK_DEVICE_NAME "ESP32"
#define BLYNK_TEMPLATE_NAME "Zzz"
#define BLYNK_AUTH_TOKEN "oPZ4J8RO_KeCwQaVns9sq-aYuQiWGDeo"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Somethinglikefatboi";
char pass[] = "00000000";

// -------- PIN --------
#define SW0_PIN 32
#define SW1_PIN 33
#define SW2_PIN 25
#define SW3_PIN 14
#define SW4_PIN 12
#define SW5_PIN 13
#define SW6_PIN 18
#define SW7_PIN 19


int Tempp = 0;
int Humid = 0;
int last_sw0 = 1, last_sw1 = 1, last_sw2 = 1, last_sw3 = 1;
int last_sw4 = 1, last_sw5 = 1, last_sw6 = 1, last_sw7 = 1;
BlynkTimer timer;


void sendDataToBlynk() {
  Blynk.virtualWrite(V23, Tempp);   
  Blynk.virtualWrite(V24, Humid);  
}


void checkSwitches() {
  int sw0 = digitalRead(SW0_PIN);
  int sw1 = digitalRead(SW1_PIN);
  int sw2 = digitalRead(SW2_PIN);
  int sw3 = digitalRead(SW3_PIN);
  int sw4 = digitalRead(SW4_PIN);
  int sw5 = digitalRead(SW5_PIN);
  int sw6 = digitalRead(SW6_PIN);
  int sw7 = digitalRead(SW7_PIN);

  // -------- TEMP --------
  if (sw0 == LOW && last_sw0 == HIGH) {
    Tempp = 6265;   // 👉 เปลี่ยนเป็นรหัส นศ. 4 ตัวท้าย
  }

  if (sw1 == LOW && last_sw1 == HIGH) {
    Tempp = 1234;
  }

  if (sw2 == LOW && last_sw2 == HIGH) {
    Tempp = random(1000, 3000);
  }

  if (sw3 == LOW && last_sw3 == HIGH) {
    Tempp = random(3000, 5000);
  }

  // -------- HUMID --------
  if (sw4 == LOW && last_sw4 == HIGH) {
    Humid = 6265;   // 👉 เปลี่ยนเป็นรหัส นศ. 4 ตัวท้าย
  }

  if (sw5 == LOW && last_sw5 == HIGH) {
    Humid = 5678;
  }

  if (sw6 == LOW && last_sw6 == HIGH) {
    Humid = random(5000, 7000);
  }

  if (sw7 == LOW && last_sw7 == HIGH) {
    Humid = random(7000, 9000);
  }

  // -------- UPDATE LAST STATE --------
  last_sw0 = sw0;
  last_sw1 = sw1;
  last_sw2 = sw2;
  last_sw3 = sw3;
  last_sw4 = sw4;
  last_sw5 = sw5;
  last_sw6 = sw6;
  last_sw7 = sw7;

  delay(50); // debounce
}

// -------- SETUP --------
void setup() {
  Serial.begin(115200);

  pinMode(SW0_PIN, INPUT_PULLUP);
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(SW3_PIN, INPUT_PULLUP);
  pinMode(SW4_PIN, INPUT_PULLUP);
  pinMode(SW5_PIN, INPUT_PULLUP);
  pinMode(SW6_PIN, INPUT_PULLUP);
  pinMode(SW7_PIN, INPUT_PULLUP);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, sendDataToBlynk);
}

// -------- LOOP --------
void loop() {
  Blynk.run();
  timer.run();
  checkSwitches();
}