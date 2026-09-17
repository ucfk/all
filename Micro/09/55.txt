#include <WiFi.h>
#include <PubSubClient.h>
const char* ssid = "Cgrs";
const char* password = "@@@@@@@@";
const char* mqtt_server = "broker.emqx.io";
const char* myTopic = "B6728793";
#define MSG_BUFFER_SIZE (50)
WiFiClient espClient;
PubSubClient client(espClient);
#include "DHT.h"
#define DHTPIN 15  
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;
void setup_wifi() {
 delay(10);
 Serial.print("/nConnecting to "); Serial.println(ssid);
 WiFi.mode(WIFI_STA); WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 randomSeed(micros());
 Serial.println(""); Serial.println("WiFi connected");
 Serial.println("IP address: "); Serial.println(WiFi.localIP());
}
void reconnect() {
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 String clientId = "ESP32 Client-";
 clientId += String(random(0xffff), HEX);
 if (client.connect(clientId.c_str())) {
 Serial.println("connected");
 client.subscribe(myTopic);
 client.publish(myTopic, "hello world");
 } else {
 Serial.print("failed, rc=");
 Serial.print(client.state());
 Serial.println(" try again in 5 seconds");
 delay(5000);
 }
 }
}
void setup() {
   dht.begin();
 Serial.begin(115200);
 setup_wifi();
 client.setServer(mqtt_server, 1883);
}
void loop() {
 if (!client.connected()) reconnect();
 client.loop();
 unsigned long now = millis();
 if (now - lastMsg > 2000) {
 float value1 = dht.readTemperature();
 float value2 = dht.readHumidity();
 snprintf (msg, MSG_BUFFER_SIZE, "Tempp = %0.2f, Humid = %0.2f", value1, value2);
 Serial.print("Publish message: ");
 Serial.println(msg);
 client.publish(myTopic, msg);
 lastMsg = millis();
 }
}
