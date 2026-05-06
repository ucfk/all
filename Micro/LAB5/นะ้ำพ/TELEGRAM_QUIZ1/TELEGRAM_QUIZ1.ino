#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

char ssid[] = "Cgrs";
char pass[] = "@@@@@@@@";

#define BOTtoken "8659756474:AAGk3ZO2ZQnr65xX1cFaSyJEJmv0hFicv9Y"
#define CHAT_ID "6545843941"
#define SW0_PIN 32
#define SW1_PIN 33
#define SW2_PIN 25
#define SW3_PIN 26
#define SW4_PIN 27
#define SW5_PIN 14
#define SW6_PIN 12
#define SW7_PIN 13

int Tempp = 0;
int Humid = 0;
int last_sw0 = 1, last_sw1 = 1, last_sw2 = 1, last_sw3 = 1;
int last_sw4 = 1, last_sw5 = 1, last_sw6 = 1, last_sw7 = 1;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
void sendTelegram(String msg) {
  Serial.println(msg);
  bot.sendMessage(CHAT_ID, msg, "");
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


  if (sw0 == LOW && last_sw0 == HIGH) {
    Tempp = 8793;
    sendTelegram("Tempp = " + String(Tempp));
  }

  if (sw1 == LOW && last_sw1 == HIGH) {
    Tempp = 1234;
    sendTelegram("Tempp = " + String(Tempp));
  }

  if (sw2 == LOW && last_sw2 == HIGH) {
    Tempp = random(1000, 3000);
    sendTelegram("Tempp = " + String(Tempp));
  }

  if (sw3 == LOW && last_sw3 == HIGH) {
    Tempp = random(3000, 5000);
    sendTelegram("Tempp = " + String(Tempp));
  }

  if (sw4 == LOW && last_sw4 == HIGH) {
    Humid = 8793;  
    sendTelegram("Humid = " + String(Humid));
  }

  if (sw5 == LOW && last_sw5 == HIGH) {
    Humid = 5678;
    sendTelegram("Humid = " + String(Humid));
  }

  if (sw6 == LOW && last_sw6 == HIGH) {
    Humid = random(5000, 7000);
    sendTelegram("Humid = " + String(Humid));
  }

  if (sw7 == LOW && last_sw7 == HIGH) {
    Humid = random(7000, 9000);
    sendTelegram("Humid = " + String(Humid));
  }

  last_sw0 = sw0;
  last_sw1 = sw1;
  last_sw2 = sw2;
  last_sw3 = sw3;
  last_sw4 = sw4;
  last_sw5 = sw5;
  last_sw6 = sw6;
  last_sw7 = sw7;
  delay(50);
}

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
  WiFi.begin(ssid, pass);
  client.setInsecure();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  sendTelegram("ESP32 online");
}
void loop() {
  checkSwitches();
}