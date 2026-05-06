String Cmd, strData;
int strInt, strlength, Pulse = 0, Freq = 0;
char buffer[40];

// 1. กำหนดขาสำหรับส่งสัญญาณ Pulse
const int pulsePin = 2; 

void setup() {
  Serial.begin(115200);
  
  // 2. ตั้งค่าขา pulsePin เป็น OUTPUT
  pinMode(pulsePin, OUTPUT);
  digitalWrite(pulsePin, LOW); // ให้เริ่มต้นที่สถานะ LOW
}

void loop() {
  if (Serial.available() > 0) {
    strData = Serial.readString();
    strData.trim();
    strlength = strData.length();
    Cmd = strData.substring(0, 1);

    String numStr = strData.substring(2, strlength);
    numStr.trim();
    strInt = numStr.toInt();

    if (Cmd == "F") {
      if (strInt != 0) Freq = strInt;
      sprintf(buffer, "read F = %s --> %d", strData.c_str(), strInt);
      Serial.println(buffer);
    }

    if (Cmd == "P") {
      if (strInt != 0) Pulse = strInt;
      sprintf(buffer, "read P = %s --> %d", strData.c_str(), strInt);
      Serial.println(buffer);
    }

    if (Cmd == "D") {
      sprintf(buffer, "read D = %s --> %d", strData.c_str(), strInt);
      Serial.println(buffer);
    }

    // 3. ปรับปรุงคำสั่ง "G" ให้ส่งสัญญาณ Pulse จริง
    if (Cmd == "G") {
      Serial.println("GO! Sending signal...");
      
      if (Freq > 0 && Pulse > 0) {
        // คำนวณหาคาบเวลา (Period) ในหน่วยมิลลิวินาที
        // T = 1/f * 1000
        int delayTime = 1000 / (Freq * 2); 

        for (int i = 0; i < Pulse; i++) {
          digitalWrite(pulsePin, HIGH);
          delay(delayTime);
          digitalWrite(pulsePin, LOW);
          delay(delayTime);
        }
        Serial.println("Pulse Finished.");
      } else {
        Serial.println("Error: Please set Freq and Pulse > 0");
      }
    }

    if (Cmd == "?") {
      Serial.println("Help: Commands = F=Hz, P=count, G, ?");
      Serial.println("Version: 1.1 - Added Pulse Output");
    }

    sprintf(buffer, "Pulse=%d, Freq=%d, Cmd=%s", Pulse, Freq, Cmd.c_str());
    Serial.println(buffer);
  }
}