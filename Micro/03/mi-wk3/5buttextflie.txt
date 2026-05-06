#include "LedController.hpp" 
#include <NTPClient.h> 
#include <WiFi.h> 
#include <WiFiUdp.h> 
#define Pin_DIN 2   
#define Pin_CS 4   
#define Pin_CLK 5
LedController<1, 1> lc;    
const char* ssid  = "Cgrs"; 
const char* password = "@@@@@@@@";
 
WiFiUDP ntpUDP; 
NTPClient timeClient(ntpUDP); 
 
void setup() { 
  lc = LedController<1, 1>(Pin_DIN, Pin_CLK, Pin_CS); 
  lc.setIntensity(8); 
  lc.clearMatrix();
  Serial.begin(115200); 
  WiFi.begin(ssid, password); 
  while ( WiFi.status() != WL_CONNECTED ) 
  { delay ( 500 ); 
    Serial.print ( "." ); 
  } 
  timeClient.begin(); 
  timeClient.setTimeOffset(25200);  // +7Hour = (7H x 60Min x 60Sec = 25200 Sec) 
  Serial.println(); 
} 
void loop() { 
  timeClient.update(); 
  Serial.print("Time -> ");   Serial.print(timeClient.getEpochTime()); 
  Serial.print(" -> ");   Serial.println(timeClient.getFormattedTime()); 
  int hour = timeClient.getHours();    // ดึงเลขชั่วโมง
  int minute = timeClient.getMinutes(); // ดึงเลขนาที
  int second = timeClient.getSeconds(); // ดึงเลขวินาที
  lc.setDigit(0, 7, hour/10, false);
  lc.setDigit(0, 6, hour%10, false);
  lc.setChar(0, 5, '-', false);
  lc.setDigit(0, 4, minute/10, false);
  lc.setDigit(0, 3, minute%10, false);
  lc.setChar(0, 2, '-', false);
  lc.setDigit(0, 1, second/10, false);
  lc.setDigit(0, 0, second%10, false);
  delay(1000); 
} 