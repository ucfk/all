#include <TM1638lite.h>

#define TM_STB  5
#define TM_CLK  18
#define TM_DIO  23

#define SW_BOOT  0
#define SW1      25
#define SW2      27

TM1638lite tm(TM_STB, TM_CLK, TM_DIO);

volatile long count   = 0;        // เปลี่ยนเป็น longs
volatile bool updated = false;

volatile unsigned long tBoot = 0;
volatile unsigned long t1    = 0;
volatile unsigned long t2    = 0;
const unsigned long DB = 200;

void showNumber(long n) {
  if (n < 0)        n = 0;
  if (n > 99999999) n = 99999999;

  tm.reset();

  for (int pos = 7; pos >= 0; pos--) {
    if (n > 0 || pos == 7) {
      tm.displayASCII(pos, '0' + (n % 10));
      n /= 10;
    }
  }
}

void IRAM_ATTR isrBoot() {
  unsigned long now = millis();
  if (now - tBoot < DB) return;
  tBoot = now;
  if (count < 99999999) count++;
  updated = true;
}

void IRAM_ATTR isr1() {
  unsigned long now = millis();
  if (now - t1 < DB) return;
  t1 = now;
  if (count < 99999999) count++;
  updated = true;
}

void IRAM_ATTR isr2() {
  unsigned long now = millis();
  if (now - t2 < DB) return;
  t2 = now;
  if (count > 0) count--;
  updated = true;
}

void setup() {
  Serial.begin(115200);

  pinMode(SW_BOOT, INPUT_PULLUP);
  pinMode(SW1,     INPUT_PULLUP);
  pinMode(SW2,     INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SW_BOOT), isrBoot, FALLING);
  attachInterrupt(digitalPinToInterrupt(SW1),     isr1,    FALLING);
  attachInterrupt(digitalPinToInterrupt(SW2),     isr2,    FALLING);

  tm.reset();
  showNumber(0);

  Serial.println("BOOT/SW1=นับขึ้น | SW2=นับลง | MAX=99999999");
}

void loop() {
  if (updated) {
    updated = false;
    long val = count;
    showNumber(val);
    Serial.print("Count = ");
    Serial.println(val);
  }
}