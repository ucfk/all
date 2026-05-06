#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"


const char* ssid = "Cgrs";
const char* password = "@@@@@@@@";
const char* mqtt_server = "test.mosquitto.org";
const char* topic1 = "B6728793_Kit";

#define DHTPIN 15  
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

int ledPins[] = { 2, 4, 5, 18 }; 
int sw1 = 13;                       
int sw2 = 12;                       

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];

// --- ฟังก์ชัน Callback: รับคำสั่งจาก MQTT เพื่อเปิด-ปิดไฟ (ข้อ 4.3) ---
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] Command: ");
  Serial.println(message);

  // เงื่อนไขเปิด-ปิดไฟ LED 4 ดวง
  if (message == "led1-on") digitalWrite(ledPins[0], HIGH);
  if (message == "led1-off") digitalWrite(ledPins[0], LOW);

  if (message == "led2-on") digitalWrite(ledPins[1], HIGH);
  if (message == "led2-off") digitalWrite(ledPins[1], LOW);

  if (message == "led3-on") digitalWrite(ledPins[2], HIGH);
  if (message == "led3-off") digitalWrite(ledPins[2], LOW);

  if (message == "led4-on") digitalWrite(ledPins[3], HIGH);
  if (message == "led4-off") digitalWrite(ledPins[3], LOW);
}

void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(topic1);  // Subscribe เพื่อรอรับคำสั่งควบคุมไฟ
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();  // เริ่มทำงานเซนเซอร์ DHT

  // ตั้งค่า Mode ของขาต่างๆ
  for (int i = 0; i < 4; i++) pinMode(ledPins[i], OUTPUT);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  long now = millis();

  // --- ข้อ 4.2: อ่านค่า DHT-22 และส่งทุก 2 วินาที ---
  if (now - lastMsg > 2000) {
    lastMsg = now;
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      snprintf(msg, 50, "Tempp: %.1f C, Humid: %.1f %%", t, h);
      Serial.print("Publish: ");
      Serial.println(msg);
      client.publish(topic1, msg);
    }
  }

  // --- ข้อ 4.4: รับค่าสวิตช์และส่งแจ้งเตือน Alarm ---
  if (digitalRead(sw1) == LOW) {  // เมื่อกด SW1
    client.publish(topic1, "Overheat Alarm");
    Serial.println("Sent: Overheat Alarm");
    while (digitalRead(sw1) == LOW)
      ;  // รอจนกว่าจะปล่อยปุ่ม
    delay(100);
  }

  if (digitalRead(sw2) == LOW) {  // เมื่อกด SW2
    client.publish(topic1, "Intruders Alarm");
    Serial.println("Sent: Intruders Alarm");
    while (digitalRead(sw2) == LOW)
      ;
    delay(100);
  }
}
