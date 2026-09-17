// Include Libraries
#include <WiFi.h>        // For connecting ESP32 to WiFi
#include <ArduinoOTA.h>  // For enabling over the air updates
#define LED_BUILTIN  2
// Set WiFi Credentials
const char* ssid = "Jerrynotmeright";      // Change to your WiFi Network name
const char* password = "0619219133";  // Change to you password

void setupOTA() {
  ArduinoOTA.setHostname("esp32-ota-test");

  ArduinoOTA.onStart([]() {
    Serial.println("Start OTA");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd OTA");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress * 100) / total);
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
  });

  ArduinoOTA.begin();
}

void setup() {
  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    delay(3000);
    ESP.restart();
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  setupOTA();
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  ArduinoOTA.handle();  // Handles OTA
  digitalWrite(LED_BUILTIN,HIGH); delay(500);
  digitalWrite(LED_BUILTIN,LOW); delay(500);

  //digitalWrite(LED_BUILTIN,HIGH); delay(250);
  //digitalWrite(LED_BUILTIN,LOW); delay(250);
  //digitalWrite(LED_BUILTIN,HIGH); delay(1000);
  //digitalWrite(LED_BUILTIN,LOW); delay(1000);


}