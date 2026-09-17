#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
//คำสั่ง เปิดเซิร์ฟเวอร์ในterminal กดเปิดในโฟลเดอร์ที่มีไฟล์.bin python -m http.server 8000 ถ้าจะปิด กด ctrl+c
const char* ssid = "RUTRADA";
const char* password = "0894280008";

// URL firmware (.bin) อยากได้ Blink ไหนเปลี่ยนเลขเอา
const char* firmware_url = "http://192.168.1.169:8000/Blink01.ino.bin";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  doUpdate();
}

void doUpdate() {
  HTTPClient http;
  http.begin(firmware_url);

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();
    WiFiClient * stream = http.getStreamPtr();

    if (Update.begin(contentLength)) {
      Serial.println("Start updating...");

      size_t written = Update.writeStream(*stream);

      if (written == contentLength) {
        Serial.println("Written successfully");
      } else {
        Serial.println("Write failed");
      }

      if (Update.end()) {
        Serial.println("Update complete. Rebooting...");
        ESP.restart();
      } else {
        Serial.println("Update not finished");
      }
    } else {
      Serial.println("Not enough space");
    }
  } else {
    Serial.printf("HTTP error: %d\n", httpCode);
  }

  http.end();
}

void loop() {}