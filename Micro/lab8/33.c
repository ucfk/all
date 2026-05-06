#include <reg51.h>

__sbit __at (0x91) Left_Select;   
__sbit __at (0x92) Right_Select;  
__sbit __at (0xB2) SW_UP;         
__sbit __at (0xB3) SW_DOWN;       

#define Segment_Port P2

unsigned char Data_Count; 
unsigned char code CODE_TABLE[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void Scan_Delay() {
    unsigned int i;
    for(i=0; i<300; i++); 
}

void Show_2Digits() {
    unsigned char tens, units;
    tens  = Data_Count / 10;
    units = Data_Count % 10;

    Segment_Port = CODE_TABLE[tens];
    Left_Select = 0; Right_Select = 1;
    Scan_Delay();
    Left_Select = 1;

    Segment_Port = CODE_TABLE[units];
    Right_Select = 0; Left_Select = 1;
    Scan_Delay();
    Right_Select = 1;
}

void External0_ISR(void) __interrupt (0) {
    unsigned int d;

    if (SW_UP == 0) {

        if (Data_Count >= 99) Data_Count = 0;
        else Data_Count++;

        for(d=0; d<40; d++) Show_2Digits(); 

        while(SW_UP == 0) {
            Show_2Digits(); 
        }
        

        for(d=0; d<10; d++) Show_2Digits();
    }
}


void External1_ISR(void) __interrupt (2) {
    unsigned int d;
    
    if (SW_DOWN == 0) {
        if (Data_Count == 0) Data_Count = 99;
        else Data_Count--;

        for(d=0; d<40; d++) Show_2Digits(); 

        while(SW_DOWN == 0) {
            Show_2Digits();
        }
        
        for(d=0; d<10; d++) Show_2Digits();
    }
}

void main() {
    Data_Count = 99; // start

    IT0 = 1; 
    IT1 = 1;   
    EX0 = 1;   
    EX1 = 1;   
    EA  = 1;   

    while(1) {
        Show_2Digits();
    }
}