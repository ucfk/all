#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"
#define pinDHT22 15
DHTesp dht;
const char* ssid = "Tucklyz";
const char* password = "pond17399";
const char* mqtt_server = "broker.emqx.io";
const char *topic = "Test";
const char* myTopic = "Pung";
const int mqtt_port = 1883;

#define LedPin1 16
#define LedPin2 17

#define MSG_BUFFER_SIZE (50)
WiFiClient espClient;
PubSubClient client(espClient);
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
      client.publish(myTopic, "LET'S GO BRATHER");
      Serial.println("Connected");
      client.subscribe("/LedControl");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");

  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.println(msg);

  if (msg == "1") {
    digitalWrite(LedPin1, HIGH); // เปิด LED
    Serial.println("LED is ON");
  } else if (msg == "0") {
    digitalWrite(LedPin1, LOW); // ปิด LED
    Serial.println("LED is OFF");
  }
  if (msg == "2") {
    digitalWrite(LedPin2, HIGH); // เปิด LED
    Serial.println("LED is ON");
  } else if (msg == "3") {
    digitalWrite(LedPin2, LOW); // ปิด LED
    Serial.println("LED is OFF");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LedPin1, OUTPUT);
  pinMode(LedPin2, OUTPUT);
  dht.setup(pinDHT22, DHTesp::DHT22);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  while (!client.connected()) {
    reconnect();
  }

  client.publish("/LedControl", "ON");
  client.subscribe("/LedControl");
}
void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    Serial.println("-------------------------------------------------------");
    snprintf (msg, MSG_BUFFER_SIZE, "Tempp = %0.2f, Humid = %0.2f", temperature, humidity);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(myTopic, msg);
    lastMsg = millis();
    delay(1500);
  }
}
