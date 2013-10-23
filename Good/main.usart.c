/* Compile options:  -ml (Large code model) */
//#include <p18f452.h>
//#include <p18f25k22.h>
#include <p18f452.h>
#include <usart.h>
#include <delays.h>

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
unsigned char x = 0;

void delay() {
    Delay10KTCYx(0xff);
    Delay10KTCYx(0xff);
    Delay10KTCYx(0xff);
    Delay10KTCYx(0xff);
}

/*void setupUSART() {
    TRISCbits.RC6 = 0;
    TRISCbits.RC7 = 0;
    TXSTAbits.SYNC = 0;
    PIE1bits.TXIE = 1;
    TXSTAbits.TXEN = 1;
    TXSTAbits.BRGH = 1;
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    RCSTAbits.ADDEN = 0;
    SPBRG = 9;
}

void transmitUSART() {
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
}
void sendUSART(char data) {
    transmitUSART();
    while(!PIR1bits.TXIF);
    TXREG = data;
}

void main() {
    char c = 'A';
    TRISCbits.RC7 = 0;
    setupUSART();
    while (1) {
        //sendUSART(0b01010101);
        //sendUSART(0b01111110);
        sendUSART(c++);
        //transmitUSART(0x00);
        //
        //delay();
    }
}*/

void main() {
    char c = 'a';
    char str[] = "hello world";
    /*OpenUSART(USART_TX_INT_OFF &
              USART_RX_INT_OFF &
              USART_ASYNCH_MODE &
              USART_EIGHT_BIT &
              USART_CONT_RX &
              USART_BRGH_HIGH &
              USART_ADDEN_OFF,
              12);*/
    OpenUSART(USART_TX_INT_OFF &
              USART_RX_INT_OFF &
              USART_ASYNCH_MODE &
              USART_EIGHT_BIT &
              USART_CONT_RX &
              USART_BRGH_HIGH &
              USART_ADDEN_OFF,
              12);
    while (1) {
        while(BusyUSART());
        //putcUSART(0b11111111);
        //delay();
        putcUSART(0xff);
        //delay();
        //if (DataRdyUSART()) {
            //c = getcUSART();
        //
        //putcUSART('U');
        //delay();
        putcUSART('A');
        putcUSART(0x00);
        //delay();
        putcUSART(0b01010101);
        //delay();
        //putsUSART("\033[2J");
        //Delay10TCYx(0xff);
    }
}