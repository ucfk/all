#include <WiFi.h>
#include <WebServer.h>

// ==========================================
// 1. ตั้งค่า Hardware Pins
// ==========================================
// LED ภายนอก 8 ดวง
const int ledPins[8] = {13,12,14,27,26,25,33,32};
// Switch ภายนอก 8 ตัว
const int swPins[8] = {15, 23, 4, 5, 18, 19,21, 22};

// ขาควบคุม TM1638 (ย้ายใหม่เพื่อไม่ให้ชนกับ Switch)
const int STB = 20; 
const int CLK = 17;
const int DIO = 16;

// ตัวแปรเก็บสถานะ
bool ledStates[8] = {false, false, false, false, false, false, false, false};
int prevSwVals[8] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long lastDebounceTime = 0;

WebServer server(80);

// ข้อมูล Wi-Fi
const char* ssid = "RUTRADA";
const char* password = "0894280008";

// ==========================================
// 2. ฟังก์ชันพื้นฐาน TM1638
// ==========================================
void tmSendByte(byte data) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(CLK, LOW);
    digitalWrite(DIO, (data >> i) & 1);
    digitalWrite(CLK, HIGH);
  }
}

void tmSendCommand(byte cmd) {
  digitalWrite(STB, LOW);
  tmSendByte(cmd);
  digitalWrite(STB, HIGH);
}

void tmSetLed(int pos, bool state) {
  tmSendCommand(0x44); // Fixed address mode
  digitalWrite(STB, LOW);
  tmSendByte(0xC1 + (pos * 2));
  tmSendByte(state ? 1 : 0);
  digitalWrite(STB, HIGH);
}

// ==========================================
// 3. ฟังก์ชันอัปเดตระบบ
// ==========================================
void updateHardware() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(ledPins[i], ledStates[i] ? HIGH : LOW); // LED ภายนอก
    tmSetLed(i, ledStates[i]); // LED บน TM1638
  }
}

void handleRoot() {
  if (server.hasArg("t")) {
    int idx = server.arg("t").toInt();
    if (idx >= 0 && idx < 8) {
      ledStates[idx] = !ledStates[idx];
      updateHardware();
    }
    server.sendHeader("Location", "/");
    server.send(303);
    return;
  }

  String html = "<html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>button{padding:15px; margin:5px; font-size:18px; width:120px; border-radius:8px; border:none; color:white; cursor:pointer;}";
  html += ".on{background-color:#4CAF50;} .off{background-color:#f44336;}</style></head>";
  html += "<body style='text-align: center; font-family: sans-serif;'><h1>Smart Home Control</h1>";
  
  for (int i = 0; i < 8; i++) {
    String status = ledStates[i] ? "ON" : "OFF";
    String btnClass = ledStates[i] ? "on" : "off";
    html += "<div style='margin-bottom:15px;'>LED " + String(i + 1) + " (Pin " + String(ledPins[i]) + "): ";
    html += "<a href='/?t=" + String(i) + "'><button class='" + btnClass + "'>" + status + "</button></a></div>";
  }
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// ==========================================
// 4. Setup & Loop
// ==========================================
void setup() {
  Serial.begin(115200);

  // กำหนดโหมด Pin
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(swPins[i], INPUT_PULLUP);
  }
  pinMode(STB, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DIO, OUTPUT);
  digitalWrite(STB, HIGH);

  // เริ่มต้น TM1638
  tmSendCommand(0x8f); // เปิดหน้าจอ
  updateHardware();

  // เชื่อมต่อ WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nIP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();

  // ตรวจสอบปุ่มกดทั้ง 8 ปุ่ม (Step 1 & 3)
  if (millis() - lastDebounceTime > 200) {
    for (int i = 0; i < 8; i++) {
      int swVal = digitalRead(swPins[i]);
      if (swVal == LOW && prevSwVals[i] == HIGH) {
        ledStates[i] = !ledStates[i]; // กดติดกดดับ
        updateHardware();
        lastDebounceTime = millis();
      }
      prevSwVals[i] = swVal;
    }
  }
}