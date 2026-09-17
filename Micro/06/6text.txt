int s1 = 13;  
int s2 = 12;  
int s3 = 14;  
int s4 = 27;  
int led[] = { 21, 19, 18, 5, 4, 2,15 }; //gfedcba
int wr[] = { 19, 4, 5, 18, 21, 2,15 }; //gfedcba
int l1=26;
int l2=25;
int l3=33;
int l4=32;
int SegmentTable[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 
0x6D, 0x7D, 0x07, 0x7F, 0x6F ,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00,0x40
};
int id[]={11,16,6,16,7,16,2,16,8,16,7,16,9,16,3,16};
int nCounter = 0; 
//8 abgecdf
#define minData 0 
#define maxData 15
#define s1 13
#define s2 12
#define s3 14
#define s4 27
int last_sw1 = 1, last_sw2 = 1, last_sw3 = 1,last_sw4 = 1;
void Display_1Digit_7Segment(int iData) { 
int SegDecode = ~SegmentTable[iData];
for (int i = 0; i < 7; i++) { 
digitalWrite(led[6-i], SegDecode & 1); 
SegDecode = SegDecode >> 1; 
  }
} 
void slow(int iData) { 
int SegDecode = ~SegmentTable[iData];
if(iData==0){for (int i = 0; i < 7; i++) { 
digitalWrite(led[6-i], SegDecode & 1); 
if((digitalRead(s1)+digitalRead(s2)+digitalRead(s3)+digitalRead(s4))==4) break;
delay(500);
SegDecode = SegDecode >> 1; 
    }
}else{for (int i = 0; i < 7; i++) { 
digitalWrite(wr[6-i], SegDecode & 1); 
if((digitalRead(s1)+digitalRead(s2)+digitalRead(s3)+digitalRead(s4))==4) break;
delay(500);
SegDecode = SegDecode >> 1; 
    }
  }
} 

void fast(int iData) { 
int SegDecode = ~SegmentTable[iData];
if(iData==0){for (int i = 0; i < 7; i++) { 
digitalWrite(led[6-i], SegDecode & 1); 
if((digitalRead(s1)+digitalRead(s2)+digitalRead(s3)+digitalRead(s4))==4) break;
delay(200);
SegDecode = SegDecode >> 1; 
    }
}else{for (int i = 0; i < 7; i++) { 
digitalWrite(wr[6-i], SegDecode & 1); 
if((digitalRead(s1)+digitalRead(s2)+digitalRead(s3)+digitalRead(s4))==4) break;
delay(200);
SegDecode = SegDecode >> 1; 
    }
  }
} 
void setup() { 
  delay(1000);
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);
  pinMode(s4, INPUT_PULLUP);
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(l3, OUTPUT);
  pinMode(l4, OUTPUT);
  for (int i = 0; i < 8; i++){
    pinMode(led[i], OUTPUT);
  }

  digitalWrite(l1,1);
  digitalWrite(l2,1);
  digitalWrite(l3,1);
  digitalWrite(l4,1);
  for (int i = 0; i < 32; i++){
  Display_1Digit_7Segment(id[nCounter]); 
  nCounter++; 
  nCounter = nCounter < minData ? 15 : nCounter; 
  nCounter = nCounter > 15 ? minData : nCounter; 
  delay(300);
  }
  digitalWrite(l1,0);
  digitalWrite(l2,0);
  digitalWrite(l3,0);
  digitalWrite(l4,0);
  while((digitalRead(s1)+digitalRead(s2)+digitalRead(s3)+digitalRead(s4))==4){
  Display_1Digit_7Segment(17);}
}

void checkSwitches() {
  int sw1 = digitalRead(s1);
  int sw2 = digitalRead(s2);
  int sw3 = digitalRead(s3);
  int sw4 = digitalRead(s4);
  int c = (sw1+sw2+sw3+sw4);

  if (c==3 && last_sw1==1 && last_sw2==1 && last_sw3==1 && last_sw4==1) {
      Display_1Digit_7Segment(16);
      digitalWrite(l1, !sw1);
      digitalWrite(l2, !sw2);
      digitalWrite(l3, !sw3);
      digitalWrite(l4, !sw4);
      if (sw1 == 0) slow(0);
      if (sw2 == 0) fast(0);
      if (sw3 == 0) fast(8);
      if (sw4 == 0) slow(8);
  }
  if (sw1 == 1 && sw2 == 1 && sw3 == 1 && sw4 == 1) {
      digitalWrite(l1,0);
      digitalWrite(l2,0);
      digitalWrite(l3,0);
      digitalWrite(l4,0);
      Display_1Digit_7Segment(16);
  }
  last_sw1 = sw1;
  last_sw2 = sw2;
  last_sw3 = sw3;
  last_sw4 = sw4;
  delay(50);

}
void loop() { 
  checkSwitches();
} 