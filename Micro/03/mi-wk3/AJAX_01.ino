
#include <WiFi.h> 
#include <WiFiClient.h> 
#include <WebServer.h> 
#include "DHTesp.h" 
 
#include "index.h"    //Our HTML webpage contents with javascripts 
#define DHT_Pin   15
#define testLED1  2
#define testLED2  4
#define testLED3  5
#define testLED4  18
 
//SSID and Password of your WiFi router 
const char* ssid  = "Cgrs"; 
const char* password = "@@@@@@@@";
 
WebServer server(80); //Server on port 80 
DHTesp dht; 
 
String ledState1 = "NA"; 
String ledState2 = "NA"; 
String ledState3 = "NA"; 
String ledState4 = "NA"; 
 
//=============================================================== 
// This routine is executed when you open its IP in browser 
//=============================================================== 
void handleRoot() { 
  String s = MAIN_page; //Read HTML contents 
  server.send(200, "text/html", s); //Send web page 
} 
 
void handle_DHT22() { 
  float h = dht.getHumidity(); 
  float t = dht.getTemperature(); 
  String tmpValue = "Temp = "; 
  tmpValue += String(t) + " C, Humidity = "; 
  tmpValue += String(h) + " %"; 
  server.send(200, "text/plane", tmpValue); //Send value to client ajax request 
} 
 
void handleLED() { 
  String t_state = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true); 
  Serial.println(t_state); 
  if (t_state == "11")  { digitalWrite(testLED1, HIGH); ledState1 = "ON"; } //Feedback parameter   
  if (t_state == "10")  { digitalWrite(testLED1, LOW);  ledState1 = "OFF";} //Feedback parameter  
  if (t_state == "21")  { digitalWrite(testLED2, HIGH); ledState2 = "ON"; } //Feedback parameter   
  if (t_state == "20")  { digitalWrite(testLED2, LOW);  ledState2 = "OFF";} //Feedback parameter  
  if (t_state == "31")  { digitalWrite(testLED3, HIGH); ledState3 = "ON"; } //Feedback parameter   
  if (t_state == "30")  { digitalWrite(testLED3, LOW);  ledState3 = "OFF";} //Feedback parameter  
  if (t_state == "41")  { digitalWrite(testLED4, HIGH); ledState4 = "ON"; } //Feedback parameter   
  if (t_state == "40")  { digitalWrite(testLED4, LOW);  ledState4 = "OFF";} //Feedback parameter  
  server.send(200, "text/plane", ledState1+", "+ledState2+", "+ledState3+", "+ledState4); //Send web page 
} 
 
void setup(void) { 
  Serial.begin(115200); 
  dht.setup(DHT_Pin, DHTesp::DHT22); 
  pinMode(testLED1, OUTPUT); 
  pinMode(testLED2, OUTPUT); 
  pinMode(testLED3, OUTPUT); 
  pinMode(testLED4, OUTPUT); 
  Serial.print("\n\nConnect to "); 
  Serial.println(ssid); 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);    Serial.print("."); 
  } 
  Serial.print("\nConnected ");   Serial.println(ssid); 
  Serial.print("IP address: ");   Serial.println(WiFi.localIP()); 
 
  server.on("/", handleRoot); 
  server.on("/setLED", handleLED); 
  server.on("/read_DHT22", handle_DHT22); 
 
  server.begin(); 
  Serial.println("HTTP server started"); 
} 
 
void loop(void) { 
  server.handleClient();          //Handle client requests 
} 