/* Compile options:  -ml (Large code model) */
//#include <p18f452.h>
//#include <p18f25k22.h>
#include <p18f452.h>

//  turn off the watch dog timer
#pragma config WDT=OFF              // Watchdog off
#pragma config BOR=OFF              // Brown out reset
#pragma config LVP=OFF               //
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
#pragma config CP2=OFF              // Code protection
#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection
#pragma config PWRT=OFF              // Power up timer off

typedef unsigned char byte;

void delay() {
    int d;
    for(d = 0; d < 10000; d++);
}

#define     PIN_WE      PORTCbits.RC4
#define     PIN_OE      PORTCbits.RC5
#define     PIN_HZ      TRISD

#define     PIN_A0      PORTCbits.RC0
#define     PIN_A1      PORTCbits.RC1
#define     PIN_A2      PORTCbits.RC2
#define     PIN_A3      PORTCbits.RC3

#define     PIN_D0      PORTDbits.RD0
#define     PIN_D1      PORTDbits.RD1
#define     PIN_D2      PORTDbits.RD2
#define     PIN_D3      PORTDbits.RD3
#define     PIN_D4      PORTDbits.RD4
#define     PIN_D5      PORTDbits.RD5
#define     PIN_D6      PORTDbits.RD6
#define     PIN_D7      PORTDbits.RD7

#define     SRAM_STORED_VALUES      16

extern byte *SRAMAddressPtrs;

void writeSRAM(byte selector, short data) {
    // Set address pins from stored value
    PIN_A0 = SRAMAddressPtrs[selector] & 0b1;
    PIN_A1 = SRAMAddressPtrs[selector] >> 1 & 0b1;
    PIN_A2 = SRAMAddressPtrs[selector] >> 2 & 0b1;
    PIN_A3 = SRAMAddressPtrs[selector] >> 3 & 0b1;

    // Set data pins on SRAM from a stored value in PIC memory
    PIN_D0 = data & 0b1;
    PIN_D1 = data >> 1 & 0b1;
    PIN_D2 = data >> 2 & 0b1;
    PIN_D3 = data >> 3 & 0b1;
    PIN_D4 = data >> 4 & 0b1;
    PIN_D5 = data >> 5 & 0b1;
    PIN_D6 = data >> 6 & 0b1;
    PIN_D7 = data >> 7 & 0b1;

    // Complete write cycle on hardware SRAM
    delay();
    PIN_OE = 0b1;
    PIN_WE = 0b0;
    PIN_HZ = 0xf;
    delay();
    PIN_OE = 0b1;
    PIN_WE = 0b1;
    PIN_HZ = 0xf;
    delay();
    PIN_OE = 0b1;
    PIN_WE = 0b1;
    PIN_HZ = 0x0;

    // Increment SRAM pointer for measurement block
    SRAMAddressPtrs[selector] = (SRAMAddressPtrs[selector] + 1) % SRAM_STORED_VALUES + (selector * SRAM_STORED_VALUES);
}

short readSRAM(byte selector, byte history) {
    short data;

    // Reads previous value
    history = (SRAMAddressPtrs[selector] + SRAM_STORED_VALUES - history) % SRAM_STORED_VALUES + (selector * SRAM_STORED_VALUES);

    // Set address pins from stored value
    PIN_A0 = history & 0b1;
    PIN_A1 = history >> 1 & 0b1;
    PIN_A2 = history >> 2 & 0b1;
    PIN_A3 = history >> 3 & 0b1;

    // Read data pins from SRAM and store value in PIC memory
    delay();
    data = PIN_D7;
    data <<= 0b1;
    data |= PIN_D6;
    data <<= 0b1;
    data |= PIN_D5;
    data <<= 0b1;
    data |= PIN_D4;
    data <<= 0b1;
    data |= PIN_D3;
    data <<= 0b1;
    data |= PIN_D2;
    data <<= 0b1;
    data |= PIN_D1;
    data <<= 0b1;
    data |= PIN_D0;

    return data;
}

void main() {
    int i, b;
    TRISC = PORTC = TRISD = PORTD = 0x0; // Enable ports for digital output
    
    for (i = 0; i < 64; i++) {
        writeSRAM(0, i);
        writeSRAM(1, ~i);
        writeSRAM(2, i ^ b);
        writeSRAM(3, ~(i ^ b++));
    }
    
    while(1) {
        for (i = 0; i < 3; i++) {
            readSRAM(i, 1);
            readSRAM(i, 2);
            readSRAM(i, 3);
            readSRAM(i, 4);
            readSRAM(i, 5);
            readSRAM(i, 6);
            readSRAM(i, 7);
        }
    }
}
/*
void ADCSelectChannel(byte channel) {
    ADCON0bits.CHS = channel;
}

void ADCRead() {
    ADCON0bits.GO_DONE = 0b1;
    while(ADCON0bits.GO_DONE != 0b0);
}

void main() {
    TRISA = 0b1111111;
    ADCON0 = 0b10000000;    // Fosc/64, channel 0 (AN0)
    ADCON1 = 0b10001111;    // Analog=AN3, AN3=V_REF+, AN2=V_REF-
    
    ADCON0bits.ADON = 0b1;
    
    TRISC = PORTC = TRISD = PORTD = 0b00000000; // Enable ports for digital output
    
    //ADCSelectChannel(0b000); // Select channel AN0
    while(1) {
        ADCRead();
        PORTC = ADRESL;
        PORTD = ADRESH;
        delay();
    }
}
/* */