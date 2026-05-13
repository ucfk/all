#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// ==========================================
// 1. ตั้งค่าเครือข่าย & Telegram
// ==========================================
const char* ssid = "Somethinglikefatboi";
const char* password = "00000000";

String BOT_TOKEN = "8716145917:AAGv1CDeIRnGQ1tIp4ZL-SPqhf9vNzexTls";
String CHAT_ID = "6912565490";

WebServer server(80);

// ==========================================
// 2. ตั้งค่า Hardware
// ==========================================
const int ledPins[4] = {21, 23, 13, 12};
const int swPins[2]  = {15, 32};

// ตัวแปรเก็บสถานะ
bool ledStates[4] = {false, false, false, false};
int swStates[2] = {HIGH, HIGH};     // สถานะปัจจุบัน (HIGH = ปล่อย, LOW = กด)
int lastSwStates[2] = {HIGH, HIGH}; // สถานะก่อนหน้า (ไว้เช็คการเปลี่ยนแปลง)

// ==========================================
// 3. ฟังก์ชันส่งข้อความเข้า Telegram
// ==========================================
void sendTelegram(String message) {
  WiFiClientSecure client;
  client.setInsecure(); // ข้ามการเช็ค Certificate เพื่อความรวดเร็วบน ESP32
  HTTPClient http;
  
  // URL สำหรับส่งข้อความ
  String url = "https://api.telegram.org/bot" + BOT_TOKEN + "/sendMessage?chat_id=" + CHAT_ID + "&text=" + message;
  
  Serial.println("Sending Telegram: " + message);
  http.begin(client, url);
  int httpCode = http.GET(); // ส่ง Request
  
  if (httpCode > 0) {
    Serial.println("Telegram Sent Successfully.");
  } else {
    Serial.println("Error sending Telegram.");
  }
  http.end();
}

// ==========================================
// 4. ฟังก์ชันจัดการหน้าเว็บ
// ==========================================
void handleRoot() {
  // --- Step 1: ตรวจสอบการกดปุ่มคุม LED จากเว็บ ---
  if (server.hasArg("led")) {
    int idx = server.arg("led").toInt();
    if (idx >= 0 && idx < 4) {
      ledStates[idx] = !ledStates[idx];
      digitalWrite(ledPins[idx], ledStates[idx] ? HIGH : LOW);
    }
    server.sendHeader("Location", "/");
    server.send(303);
    return;
  }

  // --- สร้างหน้า HTML ---
  String html = "<html><head><meta charset='utf-8'><title>Smart Remote</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  // รีเฟรชหน้าเว็บทุกๆ 3 วินาทีเพื่ออัปเดตสถานะสวิตช์อัตโนมัติ
  html += "<meta http-equiv='refresh' content='3'>"; 
  html += "<style>button{padding:15px; margin:5px; font-size:18px; width:120px; border-radius:8px; border:none; color:white; cursor:pointer;}";
  html += ".on{background-color:#4CAF50;} .off{background-color:#f44336;}</style></head>";
  html += "<body style='text-align: center; font-family: sans-serif;'><h1>Smart Dashboard</h1>";
  
  // โซนควบคุม LED (Step 1)
  html += "<h2>1. LED Control</h2>";
  for (int i = 0; i < 4; i++) {
    String status = ledStates[i] ? "ON" : "OFF";
    String btnClass = ledStates[i] ? "on" : "off";
    html += "<p>LED " + String(i + 1) + " : ";
    html += "<a href='/?led=" + String(i) + "'><button class='" + btnClass + "'>" + status + "</button></a></p>";
  }

  html += "<hr style='width:80%;'>";

  // โซนแสดงสถานะสวิตช์ (Step 2)
  html += "<h2>2. Switch Status</h2>";
  for (int i = 0; i < 2; i++) {
    // โหมด PULL_UP: ค่า LOW = กดสวิตช์ (ON), HIGH = ปล่อยสวิตช์ (OFF)
    String swStatus = (digitalRead(swPins[i]) == LOW) ? "<b style='color:green'>ON</b>" : "<b style='color:red'>OFF</b>";
    html += "<p>Switch " + String(i + 1) + " is : " + swStatus + "</p>";
  }

  html += "<p style='font-size:12px; color:gray;'>* Auto-refresh every 3 sec</p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// ==========================================
// 5. Setup & Loop
// ==========================================
void setup() {
  Serial.begin(115200);

  // ตั้งค่า Pin
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = 0; i < 2; i++) {
    pinMode(swPins[i], INPUT_PULLUP);
  }

  // เชื่อมต่อ WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nIP Address: " + WiFi.localIP().toString());

  // เริ่ม Web Server
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient(); // รอรับคำสั่งจาก Web Server

  // --- Step 3: ตรวจสอบสถานะ Switch เพื่อแจ้งเตือน Telegram ---
  for (int i = 0; i < 2; i++) {
    swStates[i] = digitalRead(swPins[i]);

    // ตรวจจับการเปลี่ยนแปลง (Edge Detection)
    if (swStates[i] != lastSwStates[i]) {
      delay(50); // Debounce ป้องกันการกระเพื่อมของสวิตช์
      
      // อ่านค่าซ้ำเพื่อความชัวร์
      if (digitalRead(swPins[i]) == swStates[i]) {
        String stateText = (swStates[i] == LOW) ? "Turned ON 🟢" : "Turned OFF 🔴";
        String msg = "🔔 Alert: Switch " + String(i + 1) + " was " + stateText;
        
        // ส่งแจ้งเตือน
        sendTelegram(msg);
        
        // อัปเดตสถานะล่าสุด
        lastSwStates[i] = swStates[i];
      }
    }
  }
}