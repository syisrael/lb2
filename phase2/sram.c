/* Compile options:  -ml (Large code model) */
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

#define     PIN_WE      PORTCbits.RA4
#define     PIN_OE      PORTCbits.RA5
#define     PIN_HZ      TRISD

#define     PIN_A0      PORTCbits.RB0
#define     PIN_A1      PORTCbits.RB1
#define     PIN_A2      PORTCbits.RB2
#define     PIN_A3      PORTCbits.RB3
#define     PIN_A4      PORTCbits.RB4
#define     PIN_A5      PORTCbits.RB5
#define     PIN_A6      PORTCbits.RA0
#define     PIN_A7      PORTCbits.RA1
#define     PIN_A8      PORTCbits.RA2
#define     PIN_A9      PORTCbits.RA3

#define     PIN_D0      PORTDbits.RD0
#define     PIN_D1      PORTDbits.RD1
#define     PIN_D2      PORTDbits.RD2
#define     PIN_D3      PORTDbits.RD3
#define     PIN_D4      PORTDbits.RD4
#define     PIN_D5      PORTDbits.RD5
#define     PIN_D6      PORTDbits.RD6
#define     PIN_D7      PORTDbits.RD7

#define     SRAM_STORED_VALUES      16

typedef unsigned char byte;
short

void setBusSRAM() {

}
void setBusPIC() {

}

void writeSRAM(byte selector, short data) {
    setBusPIC();
    delay();
    
    // Set address pins from stored value
    PIN_A0 = SRAMAddressPtrs[selector] & 0b1;
    PIN_A1 = SRAMAddressPtrs[selector] >> 1 & 0b1;
    PIN_A2 = SRAMAddressPtrs[selector] >> 2 & 0b1;
    PIN_A3 = SRAMAddressPtrs[selector] >> 3 & 0b1;
    PIN_A4 = SRAMAddressPtrs[selector] >> 4 & 0b1;
    PIN_A5 = SRAMAddressPtrs[selector] >> 5 & 0b1;
    PIN_A6 = SRAMAddressPtrs[selector] >> 6 & 0b1;

    // Set data pins on SRAM from a stored value in PIC memory
    PIN_D0 = data & 0b1;
    PIN_D1 = (data >> 1) & 0b1;
    PIN_D2 = (data >> 2) & 0b1;
    PIN_D3 = (data >> 3) & 0b1;
    PIN_D4 = (data >> 4) & 0b1;
    PIN_D5 = (data >> 5) & 0b1;
    PIN_D6 = (data >> 6) & 0b1;
    PIN_D7 = (data >> 7) & 0b1;

    // Complete write cycle on hardware SRAM
    PIN_OE = 0b1;
    PIN_WE = 0b0;
    delay();
    PIN_OE = 0b1;
    PIN_WE = 0b1;
    delay();

    // Increment SRAM pointer for measurement block
    SRAMAddressPtrs[selector] = (SRAMAddressPtrs[selector] + 1) % SRAM_STORED_VALUES + (selector * SRAM_STORED_VALUES);
}

short readSRAM(byte selector, short history) {
    short data;
    
    // Reads previous value
    history = (SRAMAddressPtrs[selector] + SRAM_STORED_VALUES - history - 1) % SRAM_STORED_VALUES + (selector * SRAM_STORED_VALUES);

    // Set address pins from stored value
    PIN_A0 = history & 0b1;
    PIN_A1 = (history >> 1) & 0b1;
    PIN_A2 = (history >> 2) & 0b1;
    PIN_A3 = (history >> 3) & 0b1;
    PIN_A4 = (history >> 4) & 0b1;
    PIN_A5 = (history >> 5) & 0b1;
    PIN_A6 = (history >> 6) & 0b1;

    // Read data pins from SRAM and store value in PIC memory
    PIN_HZ = 0xf;
    PIN_OE = 0b0;
    setBusSRAM();
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
    delay();
    setBusPIC();
    PIN_HZ = 0x0;
    PIN_OE = 0b1;
    delay();
    
    return data;
}

void main() {
    short i = 0, b = 0;
    TRISC = PORTC = TRISD = PORTD = 0x0; // Enable ports for digital output

    /*for (i = 0; i < 16; i++) {
        writeSRAM(0, i);
    }*/
    
    while(1) {
        for (i = 0; i < 16; writeSRAM(0, i++));
        for (i = 0; i < 16; readSRAM(0, i++));
    }
}