#define Pin_RX2 16
#define Pin_TX2 17

// รายชื่อขา LED 8 ดวง
int ledPins[] = {2, 4, 5, 12, 13, 14, 15, 18};

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial2.setPins(Pin_RX2, Pin_TX2);

  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // เริ่มต้นปิดหมด
  }
  Serial.println("--- RX READY (Toggle + Master Command) ---");
}

void loop() {
  if (Serial2.available() > 0) {
    String cmd = Serial2.readStringUntil('\n');
    cmd.trim();
    
    int num = cmd.toInt(); 
    
    // 1. กรณีพิมพ์ 1-8: สลับสถานะ (Toggle)
    if (num >= 1 && num <= 8) {
      int targetPin = ledPins[num - 1];
      int currentState = digitalRead(targetPin);
      digitalWrite(targetPin, !currentState); 
      
      Serial.print("LED ");
      Serial.print(num);
      Serial.println(currentState == LOW ? " -> ON" : " -> OFF");
    } 
    // 2. กรณีพิมพ์ 9: เปิดไฟทุกดวง (Master ON)
    else if (num == 9) {
      for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], HIGH);
      }
      Serial.println("Command 9: All LEDs ON");
    }
    // 3. กรณีพิมพ์ 0: ปิดไฟทุกดวง (Master OFF)
    else if (num == 0) {
      for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], LOW);
      }
      Serial.println("Command 0: All LEDs OFF");
    }
  }
}