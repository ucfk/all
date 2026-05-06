#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h" // เพิ่ม Library DHT

#define DHTPIN 15     // ต่อสาย Data ของ DHT ที่ขา GPIO 15 (ปรับตามจริง)
#define DHTTYPE DHT22 // กำหนดชนิดเป็น DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Cgrs";
const char* password = "@@@@@@@@";
const char* mqtt_server = "broker.emqx.io";
const char* myTopic = "B6728793";
const int LED1 = 2;
const int LED2 = 4;

#define MSG_BUFFER_SIZE (50)
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];

void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  WiFi.mode(WIFI_STA); WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32-B6706265-"; // ตั้ง ID ให้ไม่ซ้ำ
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(myTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) { message += (char)payload[i]; }
  
  if (message == "LED1_ON") digitalWrite(LED1, HIGH);
  else if (message == "LED1_OFF") digitalWrite(LED1, LOW);
  else if (message == "LED2_ON") digitalWrite(LED2, HIGH);
  else if (message == "LED2_OFF") digitalWrite(LED2, LOW);
}

void setup() {
  Serial.begin(115200);
  dht.begin(); // เริ่มการทำงาน DHT
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) { // ส่งทุก 2 วินาที
    lastMsg = now;

    // อ่านค่าจริงจาก DHT
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();

    // เช็คว่าอ่านค่าสำเร็จไหม
    if (isnan(hum) || isnan(temp)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // ส่งเข้า Topic รูปแบบเดิมเพื่อให้ Node-RED ที่เขียนไว้ทำงานต่อได้เลย
    snprintf (msg, MSG_BUFFER_SIZE, "Tempp = %0.2f, Humid = %0.2f", temp, hum);
    
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(myTopic, msg);
  }
}