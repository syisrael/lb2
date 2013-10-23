/* Compile options:  -ml (Large code model) */
//#include <p18f452.h>
//#include <p18f25k22.h>
#include <p18f452.h>
#include <stdio.h>
#include "lcd.h"
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

#define     PIN_TX      PORTCbits.RC7
#define     PIN_RX      PORTCbits.RC6

char buffer [33];
char str1[33] = "Hi mom!";
char str2[33] = "Woot!!";
void main() {
    printLCD(str1, str2);
    while(1){
        Delay10KTCYx(2);
        printLCD(str2, str1);
//        sprintf (buffer, "%d plus %d is %d", 3, 2, 3+2);
    }
}