// Include Libraries
#include <WiFi.h>        // For connecting ESP32 to WiFi
#include <ArduinoOTA.h>  // For enabling over the air updates

// Set WiFi Credentials
const char* ssid = "Aaa";      // Change to your WiFi Network name
const char* password = "12459876";  // Change to you password

//variabls for blinking an LED with Millis
const int led = 2;                 // ESP32 Pin to which onboard LED is connected

void setup() {
  pinMode(led, OUTPUT);

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
  digitalWrite(led, HIGH); delay(500);
  digitalWrite(led, LOW);  delay(500);
}