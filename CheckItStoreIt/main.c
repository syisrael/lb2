/* Compile options:  -ml (Large code model) */

//  turn off the watch dog timer
#pragma config WDT=OFF              // Watchdog off
#pragma config BOR=OFF              // Brown out reset
#pragma config LVP=OFF              //
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
#pragma config CP2=OFF              // Code protection
#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection
#pragma config PWRT=OFF             // Power up timer off

#include "global.h"

int m[] = { 0xf7ca, 0x1bcc, 0x003a, 0x1010 };
boolean flagTemperatureFahrenheit = false;

void main() {
    char cmd = 0xff;
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &
              USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH &
              USART_ADDEN_OFF, 129);
    while (1) {
        terminalTask();
    }
}
