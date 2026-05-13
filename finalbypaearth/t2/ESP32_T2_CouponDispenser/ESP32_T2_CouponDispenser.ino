#include <TM1638plus.h>

/*
  T2: เครื่องจำหน่ายคูปองอัตโนมัติ ราคาคูปอง 275 บาท

  การต่อวงจร:
  - TM1638 ใช้แสดงยอดเงินหรือจำนวนเงินทอน
  - moneySw[0..7] เป็นสวิตช์ active LOW แทนเงิน 1000, 500, 100, 50, 20, 10, 5, 1 บาท
  - cancelSw เป็นสวิตช์ active LOW สำหรับยกเลิกและคืนเงินในระบบ
  - dispensePin เป็น HIGH 0.5 วินาทีเมื่อจ่ายคูปอง

  การทำงาน:
  - Step1: ตอนเริ่มโปรแกรมแสดงรหัสนักศึกษา 8 ตัวท้าย ให้แก้ 12345678 เป็นรหัสจริง
  - Step2: กดสวิตช์เงินเพื่อเพิ่มยอดและแสดงยอดเงินสะสม
  - Step3: เมื่อยอดเงิน >= 275 จะจ่ายคูปอง 1 ใบ แล้วแสดงเงินทอนแบบใช้ค่ามากสุดก่อน

  จุดที่แก้ได้:
  - เปลี่ยนรหัสนักศึกษา
  - เปลี่ยน GPIO ถ้าบอร์ด input ต่อสายต่างจากนี้
*/

#define pin_STB 18
#define pin_CLK 19
#define pin_DIO 21
#define dispensePin 22

TM1638plus tm(pin_STB, pin_CLK, pin_DIO, false);

const int price = 275;
const int values[8] = {1000, 500, 100, 50, 20, 10, 5, 1};
const byte moneySw[8] = {13, 12, 14, 27, 26, 25, 33, 32};
const byte cancelSw = 0;
bool lastMoneySw[8] = {true, true, true, true, true, true, true, true};
bool lastCancel = true;
int credit = 0;

void displayNumber(long number) {
  // แสดงตัวเลขฐานสิบปกติบน 7-segment 8 หลัก
  number = constrain(number, 0, 99999999);
  for (int pos = 7; pos >= 0; pos--) {
    tm.displayHex(pos, number % 10);
    number /= 10;
  }
}

void displayChange(int change) {
  // ลำดับหลักจากซ้ายไปขวา: 1000, 500, 100, 50, 20, 10, 5, 1
  for (byte i = 0; i < 8; i++) {
    int count = change / values[i];
    change %= values[i];
    tm.displayHex(i, count % 10);
  }
}

void dispenseCoupon() {
  // ถ้าเครื่องจริงใช้ motor/servo/relay ให้แก้โค้ดจ่ายคูปองในฟังก์ชันนี้
  digitalWrite(dispensePin, HIGH);
  delay(500);
  digitalWrite(dispensePin, LOW);
}

void completeSale() {
  // จ่ายคูปอง แสดงเงินทอน 4 วินาที แล้วกลับไปสถานะรอยอดเงิน 0
  int change = credit - price;
  dispenseCoupon();
  displayChange(change);
  delay(4000);
  credit = 0;
  displayNumber(credit);
}

void cancelSale() {
  // ยกเลิกการซื้อ: คืนยอดเงินทั้งหมดและแสดงรูปแบบจำนวนธนบัตร/เหรียญเหมือนเงินทอน
  displayChange(credit);
  delay(4000);
  credit = 0;
  displayNumber(credit);
}

void setup() {
  for (byte i = 0; i < 8; i++) pinMode(moneySw[i], INPUT_PULLUP);
  pinMode(cancelSw, INPUT_PULLUP);
  pinMode(dispensePin, OUTPUT);
  digitalWrite(dispensePin, LOW);
  tm.displayBegin();
  tm.brightness(6);

  // Step1: แก้เลขนี้เป็นรหัสนักศึกษา 8 ตัวท้าย
  displayNumber(12345678);
  delay(2500);
  displayNumber(credit);
}

void loop() {
  for (byte i = 0; i < 8; i++) {
    bool now = digitalRead(moneySw[i]);
    if (lastMoneySw[i] == HIGH && now == LOW) {
      credit += values[i];
      credit = constrain(credit, 0, 99999999);
      displayNumber(credit);
      delay(180);
      if (credit >= price) completeSale();
    }
    lastMoneySw[i] = now;
  }

  bool cancelNow = digitalRead(cancelSw);
  if (lastCancel == HIGH && cancelNow == LOW) {
    cancelSale();
    delay(180);
  }
  lastCancel = cancelNow;
}
