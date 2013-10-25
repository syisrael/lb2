#include <p18f452.h>
#include <usart.h>
#include "lcd.h"

#pragma config WDT=OFF              // Watchdog off
#pragma config BOR=OFF              // Brown out reset
#pragma config LVP=OFF              // 
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
#pragma config CP2=OFF              // Code protection
#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection

char flagDisplayOn = 0;

void main(void)
{
    char line1[16] = "booting [%]";
    char line2[16] = "asdfjkl;";
    char cmd = 0xff;
    TRISDbits.RD7 = 0;

    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &
              USART_EIGHT_BIT & USART_CONT_RX, 129);

    setupLCD();
    while (1) {
        while(!DataRdyUSART()); //wait for data
        cmd = ReadUSART();

        switch (cmd) {
            case '1':
                openLCD();
                break;
            case '2':
                closeLCD();
                break;
            case 'c':
                clearLCD();
                break;
            default:
                printLCD(line1, line2);
                break;
        }
    }
}