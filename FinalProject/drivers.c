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

char str[30];
void main(void)
{
    int i = 0;
    setupSensors();
    setupLCD();
    sprintf(str,"U R D N GIN EAR");
    setupCommunications();
    printLCD(str,str);
    setupMovement();
    //buttonSetup();
    Delay10KTCYx(10);
    while(1){
        newRead();
        usartTask();
    }
}