#define LED1 2
#define LED2 4

// ลูป 30 steps = LCM(5,3)*2 = 2 รอบสมบูรณ์
const int led1Pat[30] = {1,1,1,1,1,0,0,0,0,0, 1,1,1,1,1,0,0,0,0,0, 1,1,1,1,1,0,0,0,0,0};
const int led2Pat[30] = {1,1,1,0,0,0,1,1,1,0, 0,0,1,1,1,0,0,0,1,1, 1,0,0,0,1,1,1,0,0,0};

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  for (int i = 0; i < 30; i++) {
    digitalWrite(LED1, led1Pat[i]);
    digitalWrite(LED2, led2Pat[i]);
    delay(100);  // 1 unit = 0.1s
  }
}