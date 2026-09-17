int freq ;     // Hz
int pulseCount ; // จำนวนพัลส์
int pin = 2;         // ขาที่ใช้ output
String input = "";

void setup() {
  Serial.begin(115200);
  pinMode(pin, OUTPUT);

  Serial.println("ESP32 Signal Generator Ready");
}

void loop() {
  if (Serial.available()) {
    input = Serial.readStringUntil('\n');
    input.trim();

    // ตรวจคำสั่ง
    if (input.startsWith("F=")) {
      freq = input.substring(2).toInt();
      Serial.println("read F = F=" + String(freq));
      Serial.printf("Freq = %d, Pulse = 00, Cmd=F\n",freq);
    }

    else if (input.startsWith("P=")) {
      pulseCount = input.substring(2).toInt();
      Serial.println("read P = P=" + String(pulseCount));
      Serial.printf("Freq = %d, Pulse = %d, Cmd=P\n",freq,pulseCount);
    }

    else if (input == "G") {
      Serial.printf("Freq = %d, Pulse = %d, Cmd=G\n",freq,pulseCount);
      generateSignal();
    }

    else if (input == "?") {
      showHelp();
    }

    else {
      Serial.println("Unknown command");
    }
  }
}

void generateSignal() {
  float period = 1000.0 / freq; // ms

  for (int i = 0; i < pulseCount; i++) {
    digitalWrite(pin, HIGH);
    delay(period / 2);

    digitalWrite(pin, LOW);
    delay(period / 2);
  }

  Serial.println("Done");
}

void showHelp() {
  Serial.println("=== HELP ===");
  Serial.println("P=xxx  : Set frequency (Hz)");
  Serial.println("D=yy   : Set pulse count");
  Serial.println("G      : Generate signal");
  Serial.println("?      : Show help");
  Serial.println("Dev: YourName");
  Serial.println("Ver: 1.0");
}
