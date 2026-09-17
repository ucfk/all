#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Adafruit_AHTX0.h>
WiFiClientSecure client;
HTTPClient http;
Adafruit_AHTX0 aht;
#define sw1 15
#define sw2 13
const char* ssid = "Cgrs";
const char* password = "@@@@@@@@";
String telegramToken = "8659756474:AAGk3ZO2ZQnr65xX1cFaSyJEJmv0hFicv9Y";
String chatID = "6545843941";
char dataString[120];
bool alertTemp = false;
bool alertHum = false;

void setup() {
  Serial.begin(115200);
  if (!aht.begin()) {
    Serial.println("AHT20 not detected");
    while (1);
  }
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  sendHttpRequest("ESP32 AHT20 System Online");
}

void loop() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  float h = humidity.relative_humidity;
  float t = temp.temperature;
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print("  Humidity: ");
  Serial.println(h);
  if (digitalRead(sw1) == LOW) {
    sprintf(dataString, "Temperature = %.2f C", t);
    sendHttpRequest(dataString);
    delay(1000);
  }
  if (digitalRead(sw2) == LOW) {

    sprintf(dataString, "Humidity = %.2f %%", h);
    sendHttpRequest(dataString);

    delay(1000);
  }
  if ((t > 26 || t < 22) && !alertTemp) {
    sprintf(dataString,
    "Temperature = %.2f C\nHumidity = %.2f %%", t, h);
    sendHttpRequest(dataString);
    alertTemp = true;
  }else if ((h > 60 || h < 40) && !alertHum) {
    sprintf(dataString,
    "Temperature = %.2f C\nHumidity = %.2f %%", t, h);
    sendHttpRequest(dataString);
    alertHum = true;
  }
  if (t >= 22 && t <= 26) {
    alertTemp = false;
  }
  if (h >= 40 && h <= 60) {
    alertHum = false;
  }
  delay(2000);
}
void sendHttpRequest(String message) {

  client.setInsecure();

  String url = "https://api.telegram.org/bot" + telegramToken +
               "/sendMessage?chat_id=" + chatID +
               "&text=" + urlEncode(message);

  http.begin(client, url);

  int httpCode = http.GET();

  if (httpCode > 0) {

    Serial.println("Message Sent");
    Serial.println(http.getString());

  } else {

    Serial.print("HTTP Error: ");
    Serial.println(http.errorToString(httpCode));

  }

  http.end();
}


String urlEncode(String message) {

  String encoded = "";

  char c;

  for (int i = 0; i < message.length(); i++) {

    c = message[i];

    if (c == ' ')
      encoded += "%20";
    else if (c == '\n')
      encoded += "%0A";
    else
      encoded += c;
  }

  return encoded;
}