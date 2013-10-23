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

void delay() {
    int d;
    for(d = 0; d < 100; d++);
}

void ADCSelectChannel(char channel) {
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
