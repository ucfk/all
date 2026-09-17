#include <WiFi.h>      
#include <ArduinoOTA.h> 
const char* ssid = "RUTRADA";     
const char* password = "0894280008";  
const int led = 2;                

void setup() {
  pinMode(led, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.begin();  
}
void loop() {
  ArduinoOTA.handle();  
  digitalWrite(led, HIGH); delay(500);
  digitalWrite(led, LOW);  delay(500);
}