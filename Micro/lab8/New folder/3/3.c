#include <reg51.h>
unsigned char seg[] = {0x3F,0x06,0x5B,0x4F,0x66,
                       0x6D,0x7D,0x07,0x7F,0x6F};
volatile unsigned char count = 27;

void delay() {
    int i,j;
    for(i=0;i<5;i++)
        for(j=0;j<200;j++);
}

void int0_isr(void) interrupt 0 {
    count++;
    if(count > 99) count = 0;
}

void int1_isr(void) interrupt 2 {
    if(count == 0) count = 99;
    else count--;
}

void main() {
    unsigned char tens, ones;
    IT0 = 1;
    IT1 = 1;
    EX0 = 1;
    EX1 = 1;
    EA  = 1;

    while(1) {
        tens = count / 10;
        ones = count % 10;

        // digit 1 (หลักสิบ)  ซ้าย
        P2 = 0xFD;        //  สลับกัน
        P1 = seg[tens];
        delay();

        // digit 2 (หลักหน่วย)  ขวา
        P2 = 0xFE;        //  สลับกัน
        P1 = seg[ones];
        delay();
    }
}