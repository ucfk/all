#define Pin_RX2 16
#define Pin_TX2 17

void setup() {
  Serial.begin(115200); 
  Serial2.begin(9600);
  Serial2.setPins(Pin_RX2, Pin_TX2);
  
  Serial.println("--- TX READY (Advanced Mode) ---");
  Serial.println("Type 1-8: Toggle each LED");
  Serial.println("Type 9  : Turn ON All LEDs (New!)");
  Serial.println("Type 0  : Turn OFF All LEDs");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input.length() > 0) {
      Serial2.println(input);
      Serial.print("Sent Command: ");
      Serial.println(input);
    }
  }
}