/* Compile options:  -ml (Large code model) */
#include <p18f452.h>
#include <delays.h>
#include <stdlib.h>

#pragma config WDT=OFF              // Watchdog off
#pragma config BOR=OFF              // Brown out reset
#pragma config LVP=OFF              //
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
//#pragma config CP2=OFF              // Code protection
//#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection

#define     PIN_WE      PORTCbits.RC0
#define     PIN_OE      PORTCbits.RC1
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
short SRAMAddressPtrs[4] = {0,32,64,96};

void setupSRAM(void);
int getAnalog(void);
int getDigital(void);
void writeSRAM(byte selector, int data);
int readSRAM(byte selector, short history);

void writeSRAM(byte selector, int data) {
    PIN_OE = 0b1;
    PIN_WE = 0b1;
    Delay1KTCYx(1);
    PIN_HZ = 0x00;

    // Set address pins from stored value for MSBs of data
    PIN_A0 = SRAMAddressPtrs[selector] & 0b1;
    PIN_A1 = (SRAMAddressPtrs[selector] >> 1) & 0b1;
    PIN_A2 = (SRAMAddressPtrs[selector] >> 2) & 0b1;
    PIN_A3 = (SRAMAddressPtrs[selector] >> 3) & 0b1;
    PIN_A4 = (SRAMAddressPtrs[selector] >> 4) & 0b1;
    PIN_A5 = (SRAMAddressPtrs[selector] >> 5) & 0b1;
    PIN_A6 = (SRAMAddressPtrs[selector] >> 6) & 0b1;

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
    
    Delay1KTCYx(1);
    PIN_WE = 0b0;
    Delay10KTCYx(1);
    PIN_WE = 0b1;
    Delay10KTCYx(1);

    // Increment SRAM pointer for measurement block
    SRAMAddressPtrs[selector] = (SRAMAddressPtrs[selector] + 1) % SRAM_STORED_VALUES + (selector * SRAM_STORED_VALUES);
    
    // Set address pins from stored value for LSBs
    PIN_A0 = SRAMAddressPtrs[selector] & 0b1;
    PIN_A1 = ( SRAMAddressPtrs[selector] ) >> 1 & 0b1;
    PIN_A2 = ( SRAMAddressPtrs[selector] ) >> 2 & 0b1;
    PIN_A3 = ( SRAMAddressPtrs[selector] ) >> 3 & 0b1;
    PIN_A4 = ( SRAMAddressPtrs[selector] ) >> 4 & 0b1;
    PIN_A5 = ( SRAMAddressPtrs[selector] ) >> 5 & 0b1;
    PIN_A6 = ( SRAMAddressPtrs[selector] ) >> 6 & 0b1;
	
	// Set data pins on SRAM from a stored value in PIC memory
    //PIN_HZ = 0x00;

    PIN_D0 = (data >> 8) & 0b1;
    PIN_D1 = (data >> 9) & 0b1;
    PIN_D2 = (data >> 10) & 0b1;
    PIN_D3 = (data >> 11) & 0b1;
    PIN_D4 = (data >> 12) & 0b1;
    PIN_D5 = (data >> 13) & 0b1;
    PIN_D6 = (data >> 14) & 0b1;
    PIN_D7 = (data >> 15) & 0b1;

    // Complete write cycle on hardware SRAM
    Delay1KTCYx(1);
    PIN_WE = 0b0;
    Delay10KTCYx(1); //delay 5 ms
    PIN_WE = 0b1;
    Delay10KTCYx(1); //delay 5 ms
    SRAMAddressPtrs[selector] = (SRAMAddressPtrs[selector] + 1) % SRAM_STORED_VALUES + (selector * SRAM_STORED_VALUES);
    
}

int readSRAM(byte selector, short history) {
    int data = 0;
    int flipbuff = 0;
    PIN_WE = 0b1;
    PIN_HZ = 0xFF;
    PIN_OE = 0b0;
    Delay1KTCYx(1);
    // Reads previous value which is MSBs of the int
    history = (SRAMAddressPtrs[selector] + SRAM_STORED_VALUES - history - 2) % SRAM_STORED_VALUES + (selector * SRAM_STORED_VALUES);

    // Set address pins from stored value for first half 
    PIN_A0 = history & 0b1;
    PIN_A1 = (history >> 1) & 0b1;
    PIN_A2 = (history >> 2) & 0b1;
    PIN_A3 = (history >> 3) & 0b1;
    PIN_A4 = (history >> 4) & 0b1;
    PIN_A5 = (history >> 5) & 0b1;
    PIN_A6 = (history >> 6) & 0b1;
    // Read data pins from SRAM and store value in PIC memory
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

    Delay10KTCYx(1);
	// Moves pointer back one spot to LSBs
    history++;
	
	// Set address pins from stored value for second half 
    PIN_A0 = history & 0b1;
    PIN_A1 = (history >> 1) & 0b1;
    PIN_A2 = (history >> 2) & 0b1;
    PIN_A3 = (history >> 3) & 0b1;
    PIN_A4 = (history >> 4) & 0b1;
    PIN_A5 = (history >> 5) & 0b1;
    PIN_A6 = (history >> 6) & 0b1;

    Delay1KTCYx(1); //delay 5 ms

    data <<= 0b1;
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

    flipbuff = data >> 8;
    data <<= 8;
    data |= flipbuff;
    Delay10KTCYx(1);

    return data;
}

int getAnalog(){
	int send = rand() % 819; //identical to the ADC's value for 4 volts
	return send;
}

int getDigital(){
	int send = rand() % 4098; //equivalent to the 12 bits from the flow rate
	return send;
}



void main() {
    int i = 0, b = 0;
    setupSRAM();
    Delay1KTCYx(1);
    for (i = 0; i < 16; i++){
            //writeSRAM(0, getAnalog()); //carbon
            //writeSRAM(1, getDigital()); // flow rate
            //writeSRAM(2, getAnalog()); //salinity
            //writeSRAM(3, getAnalog()); //temp
    }
    for (i = 0; i < 32; i++){
           writeSRAM(0, i);
           writeSRAM(1, i);
           writeSRAM(2, i);
           writeSRAM(3, i);
    }
    while(1) {
        for (i = 0; i < 32; i += 2){
            readSRAM(0, i);
            readSRAM(1, i);
            readSRAM(2, i);
            readSRAM(3, i);
	}
    }
}

void setupSRAM(void)
{
    TRISCbits.RC0 = 0;
    TRISCbits.RC1 = 0;

    TRISBbits.RB0 = 0;
    TRISBbits.RB1 = 0;
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 0;
    TRISBbits.RB5 = 0;

    TRISAbits.RA0 = 0;
}