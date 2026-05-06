//https://docs.google.com/spreadsheets/d/1GgjOJFmH3CbKvp1m-9cfoMDFIVG1vr6lyHnY-v4dbzQ/edit?gid=0#gid=0
#include "WiFi.h" 
#include <HTTPClient.h> 
#include "time.h"
#include <WiFiClientSecure.h>
const char* ntpServer = "pool.ntp.org"; 
const long  gmtOffset_sec = 25200; // +7*60*60 
const int   daylightOffset_sec = 0; 
 
const char* ssid     = "Cgrs"; 
const char* password = "@@@@@@@@"; 
 
// Google script ID and required creden als 
String GOOGLE_SCRIPT_ID = "AKfycbzqp9UTKJmgACUcEEOcdEP86AkiDlJt4ll0M7zCUe27cYEcGtho4QZ4VWiAkArznGMl";    // change Gscript ID 
char timeStringBuff[50]; // 50 chars should be enough for YYYYMMDD-HHMMSS 
int nCounter = 0; 

#define SW0_PIN 32
#define SW1_PIN 33
#define SW2_PIN 25
#define SW3_PIN 26
#define SW4_PIN 27
#define SW5_PIN 14
#define SW6_PIN 12
#define SW7_PIN 13
void setup() { 

  delay(1000);
  pinMode(SW0_PIN, INPUT_PULLUP);
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(SW3_PIN, INPUT_PULLUP);
  pinMode(SW4_PIN, INPUT_PULLUP);
  pinMode(SW5_PIN, INPUT_PULLUP);
  pinMode(SW6_PIN, INPUT_PULLUP);
  pinMode(SW7_PIN, INPUT_PULLUP); 
  Serial.begin(115200); 
  Serial.println(); 
  Serial.print("Connec ng to wifi: "); 
  Serial.println(ssid); 
  Serial.flush(); 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  } 
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); 
} 
int Tempp = 0;
int Humid = 0;
int last_sw0 = 1, last_sw1 = 1, last_sw2 = 1, last_sw3 = 1;
int last_sw4 = 1, last_sw5 = 1, last_sw6 = 1, last_sw7 = 1;
void sendsheet(){
  if (WiFi.status() == WL_CONNECTED) { 
    static bool flag = false; 
    struct tm timeinfo; 
    if (!getLocalTime(&timeinfo)) { 
      Serial.println("Failed to obtain me"); 
      return; 
    } 
    strftime( timeStringBuff, sizeof( timeStringBuff), "%Y%m%d_%H%M%S", &timeinfo); 
    String asString( timeStringBuff); 
    Serial.print("Time >> "); 
    Serial.println(asString); 
    String urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + 
                      "Time=" + asString + 
                      "&Tempp=" + Tempp + 
                      "&Humid=" + Humid ; 
 
    Serial.print("POST data to spreadsheet >> "); 
    Serial.println(urlFinal); 
    HTTPClient http; 
    http.begin(urlFinal.c_str()); 
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS); 
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: "); 
    Serial.println(httpCode); 
    //--------------------------------------------------------------------- 
    //ge ng response from google sheet 
    String payload; 
    if (httpCode > 0) { 
      payload = http.getString(); 
      Serial.println("Payload: " + payload); 
    } 
    //--------------------------------------------------------------------- 
    http.end(); 
  }
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
    sendsheet();
  }

  if (sw1 == LOW && last_sw1 == HIGH) {
    Tempp = 1234;
    sendsheet();
  }

  if (sw2 == LOW && last_sw2 == HIGH) {
    Tempp = random(1000, 3000);
    sendsheet();
  }

  if (sw3 == LOW && last_sw3 == HIGH) {
    Tempp = random(3000, 5000);
    sendsheet();
  }

  if (sw4 == LOW && last_sw4 == HIGH) {
    Humid = 8793;  
    sendsheet();
  }

  if (sw5 == LOW && last_sw5 == HIGH) {
    Humid = 5678;
    sendsheet();
  }

  if (sw6 == LOW && last_sw6 == HIGH) {
    Humid = random(5000, 7000);
    sendsheet();
  }

  if (sw7 == LOW && last_sw7 == HIGH) {
    Humid = random(7000, 9000);
    sendsheet();
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
 
 
 
 
void loop() { 
  checkSwitches();
} 