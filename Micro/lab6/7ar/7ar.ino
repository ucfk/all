int pushButton = 13;  
int pushButton2 = 12;  
int led[] = {26,25,33,32, 15, 2, 19,21 }; 
volatile int lastled = 0; 
volatile int Do = 0; // 0=หยุด, 1=ซ้ายไปขวา (LtoR), 2=ขวาไปซ้าย (RtoL)

void setup() {
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);
  for (int i = 0; i < 8; i++) {
    pinMode(led[i], OUTPUT);
  }
}

void onfromL(int last) { 
  for (int i = last; i < 8; i++) { 
    digitalWrite(led[i], HIGH); 
    delay(200);
    digitalWrite(led[i], LOW);

    if (digitalRead(pushButton2) == LOW) { 
      Do = 2; 
      lastled = i; 
      return; 
    }
    if (i == 7) lastled = 0; 
  }
} 

void onfromR(int last) { 
  for (int i = last; i >= 0; i--) { 
    digitalWrite(led[i], HIGH); 
    delay(200);  
    digitalWrite(led[i], LOW);
    
    // ตรวจเช็กว่า,ีการกดปุ่,สลับโห,ดหรือไ,่
    if (digitalRead(pushButton) == LOW) { 
      Do = 1; 
      lastled = i; 
      return; 
    }
    
    // ถ้าวิ่งถึงตัวแรก ให้วนกลับไปเริ่,ที่ 7
    if (i == 0) lastled = 7;
  }  
} 

void loop() {
  // ตรวจสอบการกดปุ่,เพื่อเริ่,งาน
  if (digitalRead(pushButton) == LOW) {
    delay(50); // Debounce
    Do = 1;
  }
  if (digitalRead(pushButton2) == LOW) {
    delay(50); // Debounce
    Do = 2;
  }

  // เลือกฟังก์ชันทำงานตา,ค่าของ Do
  if (Do == 1) {
    onfromL(lastled);
  } else if (Do == 2) {
    onfromR(lastled);
  }
}