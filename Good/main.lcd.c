#include <p18f452.h>
#include <usart.h>
#include <delays.h>
#include <timers.h>
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

char str[] = "TESTING 1";
char str1[] = "START";
int i = 0;

void timer_isr (void);

#pragma code high_vector=0x08
void high_interrupt (void)
{ _asm GOTO timer_isr _endasm
}
#pragma code
#pragma interrupt timer_isr

void timer_isr (void)
{
	INTCONbits.TMR0IF = 0;
        i=1;
}
void main(void)
{
    OpenTimer0( TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_256);
    INTCONbits.GIE = 1; //enable interrupts
    printLCD(str1,str1);
    while(1){
        if(i){
            printLCD(str,str);
            Delay10KTCYx(80);
            i=0;
        }
    }
}
