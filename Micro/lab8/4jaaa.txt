#include <TM1638plus.h> 
#define  pin_STB 2
#define  pin_CLK 4 
#define  pin_DIO 5 
TM1638plus tm(pin_STB, pin_CLK , pin_DIO,false); 

struct Button { 
  const uint8_t PIN; 
  uint32_t nKeyPresses; 
  bool pressed; 
}; 
int Temp=99;
unsigned long button_time = 0; 
unsigned long last_button_time = 0; 
unsigned long button_time2 = 0; 
unsigned long last_button_time2 = 0; 
Button button1 = {22, 0, false}; 
Button button2 = {23, 0, false}; 
 
void IRAM_ATTR isr() { 
  button_time = millis(); 
  if (button_time - last_button_time > 250)  { 
    button1.nKeyPresses++; 
    button1.pressed = true; 
    last_button_time = button_time;
    Temp++; 
  } 
} 

void IRAM_ATTR isr2() { 
  button_time2 = millis(); 
  if (button_time2 - last_button_time2 > 250)  { 
    button2.nKeyPresses++; 
    button2.pressed = true; 
    last_button_time2 = button_time2;
    Temp--; 
  } 
} 
void setup() { 
  Serial.begin(115200); 
  tm.displayBegin(); 
  tm.brightness(6); 
  pinMode(button1.PIN, INPUT_PULLUP); 
  pinMode(button2.PIN, INPUT_PULLUP); 
  attachInterrupt(button1.PIN, isr, FALLING); 
  attachInterrupt(button2.PIN, isr2, FALLING); 
} 
 
void loop() { 
  if (button1.pressed) { 
    Serial.printf("Button1 has been pressed %u times\n", button1.nKeyPresses); 
    button1.pressed = false; 
  } 
  if (button2.pressed) { 
    Serial.printf("Button2 has been pressed %u times\n", button2.nKeyPresses); 
    button2.pressed = false; 
  } 
  int sing=Temp%10;  tm.displayHex(7,sing);
  int sing2=(Temp/10)%10; tm.displayHex(6,sing2);

}