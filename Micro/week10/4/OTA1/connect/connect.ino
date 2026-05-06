// Include Libraries
#include <WiFi.h>        // For connecting ESP32 to WiFi
#include <ArduinoOTA.h>  // For enabling over the air updates

// Set WiFi Credentials
const char* ssid = "RUTRADA";      // Change to your WiFi Network name
const char* password = "0894280008";  // Change to you password

void setup() {
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.begin();  // Starts OTA
}

void loop() {
  ArduinoOTA.handle();  // Handles OTA
}