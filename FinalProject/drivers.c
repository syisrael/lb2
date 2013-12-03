#include <p18f452.h>
#include <delays.h>
#include <timers.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "lcd.h"
#include "movement.h"
#include "communications.h"
#include "Sensors.h"

#pragma config WDT=OFF              // Watchdog off
#pragma config BOR=OFF              // Brown out reset
#pragma config LVP=OFF              //
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
#pragma config CP2=OFF              // Code protection
#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection

int tCounter = 0;
void timer_isr (void);

#pragma code high_vector=0x08
void high_interrupt (void)
{
	_asm GOTO timer_isr _endasm
}
#pragma code
#pragma interrupt timer_isr

void timer_isr (void)
{
	INTCONbits.TMR0IF = 0;
	tCounter++;
}

char str[30];
void main(void)
{
    int i = 0;
    long a = 10000;
    long b = -10000;
    setupSensors();
    sprintf(str,"U R D N GIN EAR");
    setupCommunications();
    printLCD(str,str);
    setupMovement();
    Delay10KTCYx(10);
    while(1){
        setupLCD();
        newRead();
        usartTask();
        retractSolenoid();
        moveTo(b,b);
        moveTo(a,-a);
        extendSolenoid();
        moveTo(a,a);
        moveTo(b,a);
        for(i=0;i<10;i++)
            Delay10KTCYx(10);
    }
}