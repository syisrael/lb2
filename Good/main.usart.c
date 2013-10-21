/* Compile options:  -ml (Large code model) */
//#include <p18f452.h>
//#include <p18f25k22.h>
#include <p18f452.h>
#include <usart.h>

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

#define     PIN_TX      PORTCbits.RC7
#define     PIN_RX      PORTCbits.RC6

void delay() {
    int d;
    for(d = 0; d < 100; d++);
}

void main() {
    short i = 0;
    OpenUSART(USART_TX_INT_OFF &
              USART_RX_INT_OFF &
              USART_ASYNCH_MODE &
              USART_EIGHT_BIT &
              USART_CONT_RX &
              USART_BRGH_HIGH &
              USART_ADDEN_OFF,
              12);
    while (1) {
        if (!BusyUSART()) {
            //putcUSART(i);
            putcUSART(0xff);
            //putsUSART("U"); //0101 0101
            //putsUSART("ª"); //1010 1010
            //putsUSART("ÿ"); //1111 1111
            //if (i % 10) {
            //    putsUSART("\033[2J");
            //}
        } else {
            i++;
        }
    }
}