/* Compile options:  -ml (Large code model) */
#include <p18f452.h>
#include <delays.h>

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

#define     PIN_WE      PORTAbits.RA4
#define     PIN_OE      PORTAbits.RA5
#define     PIN_HZ      TRISD

#define     PIN_A0      PORTBbits.RB0
#define     PIN_A1      PORTBbits.RB1
#define     PIN_A2      PORTBbits.RB2
#define     PIN_A3      PORTBbits.RB3
#define     PIN_A4      PORTBbits.RB4
#define     PIN_A5      PORTBbits.RB5

#define     PIN_A6      PORTAbits.RA0

#define     PIN_D0      PORTDbits.RD0
#define     PIN_D1      PORTDbits.RD1
#define     PIN_D2      PORTDbits.RD2
#define     PIN_D3      PORTDbits.RD3
#define     PIN_D4      PORTDbits.RD4
#define     PIN_D5      PORTDbits.RD5
#define     PIN_D6      PORTDbits.RD6
#define     PIN_D7      PORTDbits.RD7

#define     SRAM_STORED_VALUES      32

typedef unsigned char byte;
char fahrenheit = 1;
char * SRAMAddressPtrs;


void writeSRAM(byte selector, int data) {
    // Set address pins from stored value for MSBs of data
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
    Delay1KTCYx(1);
    PIN_OE = 0b1;
    PIN_WE = 0b1;
    Delay1KTCYx(1);

    // Increment SRAM pointer for measurement block
    SRAMAddressPtrs[selector] = (SRAMAddressPtrs[selector] + 1) % SRAM_STORED_VALUES + (selector * SRAM_STORED_VALUES);
    
    // Set address pins from stored value for LSBs
    PIN_A0 = SRAMAddressPtrs[selector] & 0b1;
    PIN_A1 = SRAMAddressPtrs[selector] >> 1 & 0b1;
    PIN_A2 = SRAMAddressPtrs[selector] >> 2 & 0b1;
    PIN_A3 = SRAMAddressPtrs[selector] >> 3 & 0b1;
    PIN_A4 = SRAMAddressPtrs[selector] >> 4 & 0b1;
    PIN_A5 = SRAMAddressPtrs[selector] >> 5 & 0b1;
    PIN_A6 = SRAMAddressPtrs[selector] >> 6 & 0b1;
	
	// Set data pins on SRAM from a stored value in PIC memory
    PIN_D0 = (data >> 8) & 0b1;
    PIN_D1 = (data >> 9) & 0b1;
    PIN_D2 = (data >> 10) & 0b1;
    PIN_D3 = (data >> 11) & 0b1;
    PIN_D4 = (data >> 12) & 0b1;
    PIN_D5 = (data >> 13) & 0b1;
    PIN_D6 = (data >> 14) & 0b1;
    PIN_D7 = (data >> 15) & 0b1;

    // Complete write cycle on hardware SRAM
    PIN_OE = 0b1;
    PIN_WE = 0b0;
    Delay1KTCYx(1); //delay 5 ms
    PIN_OE = 0b1;
    PIN_WE = 0b1;
    Delay1KTCYx(1); //delay 5 ms
}

int readSRAM(byte selector, short history) {
    int data = 0;

    // Reads previous value which is MSBs of the int
    history = (SRAMAddressPtrs[selector] + SRAM_STORED_VALUES - history - 1) % SRAM_STORED_VALUES + (selector * SRAM_STORED_VALUES);

    // Set address pins from stored value for first half 
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
    Delay1KTCYx(1); //delay 5 ms
	
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
	
	// Moves pointer back one spot to LSBs
    history = (SRAMAddressPtrs[selector] + SRAM_STORED_VALUES - history - 1) % SRAM_STORED_VALUES + (selector * SRAM_STORED_VALUES);
	
	// Set address pins from stored value for second half 
    PIN_A0 = history & 0b1;
    PIN_A1 = (history >> 1) & 0b1;
    PIN_A2 = (history >> 2) & 0b1;
    PIN_A3 = (history >> 3) & 0b1;
    PIN_A4 = (history >> 4) & 0b1;
    PIN_A5 = (history >> 5) & 0b1;
    PIN_A6 = (history >> 6) & 0b1;
    data <<= 8; //shift 8
    PIN_HZ = 0xf;
    PIN_OE = 0b0;
    Delay1KTCYx(1); //delay 5 ms
	
    data |= PIN_D7;
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
	
    Delay1KTCYx(1); //delay 5 ms
    PIN_HZ = 0x0;
    PIN_OE = 0b1;
    Delay1KTCYx(1); //delay 5 ms
    
    return data;
}

void main() {
    int i = 0, b = 0;
    TRISC = PORTC = TRISD = PORTD = 0x0; // Enable ports for digital output

    /*for (i = 0; i < 16; i++) {
        writeSRAM(0, i);
    }*/
    
    while(1) {
        for (i = 0; i < 16; writeSRAM(0, i++));
        for (i = 0; i < 32; i+=2){
			readSRAM(0, i);
		}
    }
}