//https://docs.google.com/spreadsheets/d/1GgjOJFmH3CbKvp1m-9cfoMDFIVG1vr6lyHnY-v4dbzQ/edit?gid=0#gid=0
#include "WiFi.h" 
#include <HTTPClient.h> 
#include "time.h"
uint64_t chipid;  
const char* ntpServer = "pool.ntp.org"; 
const long  gmtOffset_sec = 25200; // +7*60*60 
const int   daylightOffset_sec = 0; 
 
const char* ssid     = "Cgrs"; 
const char* password = "@@@@@@@@"; 
 
// Google script ID and required creden als 
String GOOGLE_SCRIPT_ID = "AKfycbznvSOZPjGjm3p_cUGlumASB2F8GsEm6T8jkc3RYYbvTV_bWA03PmF6C4dIEM9srTsZ";    // change Gscript ID 
char timeStringBuff[50]; // 50 chars should be enough for YYYYMMDD-HHMMSS 
int nCounter = 0; 
String getMacAddress() {
  uint64_t chipid = ESP.getEfuseMac();
  char macStr[18];
  // จัดรูปแบบให้เป็น AA:BB:CC:DD:EE:FF
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           (uint8_t)(chipid >> 0), (uint8_t)(chipid >> 8),
           (uint8_t)(chipid >> 16), (uint8_t)(chipid >> 24),
           (uint8_t)(chipid >> 32), (uint8_t)(chipid >> 40));
  return String(macStr);
}
void setup() { 
  delay(1000); 
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
  // Init and get the me 
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); 
} 
 
 
 
 
void loop() { 
  float internalTemp = temperatureRead();
  String macID = getMacAddress();
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
                      "Item=" + String(nCounter) + 
                      "&Date_Time=" + asString + 
                      "&WIFI_Name=" + ssid + 
                      "&MAC_Address=" + macID + 
                      "&Internal_ESP32_Tempperature=" + internalTemp+
                      "&intData_1=" + String(random(299)) + 
                      "&intData_2=" + String(random(400, 599)) + 
                      "&intData_3=" + String(random(700, 999)) + 
                      "&floatData_1=" + String(random(0, 49999) / 1000.0,3) + 
                      "&floatData_2=" + String(random(50000, 99999) / 1000.0,3); 
 
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
  nCounter++; 
  delay(1000); 
} 