#include <p18f452.h>
#include <string.h>
#include <delays.h>
#include <timers.h>
#include <usart.h>
#include <stdio.h>
#include <stdlib.h>
#include "measure.h"
#include "lcd.h"
#include "communications.h"

#pragma config WDT=OFF              // Watchdog off
#pragma config BOR=OFF              // Brown out reset
#pragma config LVP=OFF              //
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
#pragma config CP2=OFF              // Code protection
#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection

char measureAll = 1;
char fahrenheit = 1;
char str1[30] = "                ";
char str2[30] = "                ";
int tCounter = 0;
char timerFlag = 1;
void timer_isr (void);
void showMeasurements(void);

#pragma code high_vector=0x08
void high_interrupt (void) {_asm GOTO timer_isr _endasm}
#pragma code
#pragma interrupt timer_isr

void timer_isr (void)
{
	INTCONbits.TMR0IF = 0;
        timerFlag = 1;
}

void showMeasurements(){
    int i;
    int value;
    fahrenheit = PORTBbits.RB1; // F or C switch
    
    value = getTemp(fahrenheit);
    if(fahrenheit){
        if(value > 212){

            sprintf(str1,"Temp: >212F     ");
        }else if(value < 23){
            sprintf(str1,"Temp: <23F      ");
        }else{
            sprintf(str1,"Temp: %4dF     ",value);
        }
    }else{
        if(value > 100){
            sprintf(str1,"Temp: >100C     ");
        }else if(value < -5){
            sprintf(str1,"Temp: <-5C      ");
        }else{
            sprintf(str1,"Temp: %4dC     ",value);
        }
    }

    value = getSalinity();
    if(value > 50){
        sprintf(str2,"Salinity: >50ppt");
    }else if(value < 5){
        sprintf(str2,"Salinity: <5ppt ");
    }else{
        sprintf(str2,"Salinity: %4dppt",getSalinity());
    }
    printLCD(str1,str2);
    for(i=0;i<50;i++)
        Delay10KTCYx(10);

    value = getCarbon();
    if(value > 350){
        sprintf(str1,"Carbon: >350ppm         ");
    }else if(value < 10){
        sprintf(str1,"Carbon: <10ppm          ");
    }else{
        sprintf(str1,"Carbon: %4dppm         ",getCarbon());
    }
    value = getFlowRate();
    if(value > 1000){
        sprintf(str2,"Flow: >1000dLps         ");
    }else if(value < 100){
        sprintf(str2,"Flow: <100Lps           ");
    }else{
        sprintf(str2,"Flow: %4dLps           ",getFlowRate());
    }
    printLCD(str1,str2);
    for(i=0;i<50;i++)
        Delay10KTCYx(10);
    //closeLCD();
}

void main(void) {
    OpenTimer0( TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_256);
    INTCONbits.GIE = 1; //enable interrupts
    TRISBbits.RB1 = 1; //F or C switch
    setupMeasurement();
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &
              USART_EIGHT_BIT & USART_CONT_RX, 129);

    setupLCD();
    Delay1KTCYx(10);
    measureCarbon();
    measureTemperature();
    measureSalinity();
    measureFlowRate();
    updateTerminal();

    while(1){
        if(timerFlag){
            Delay1KTCYx(1);
            if(tCounter % 76 == 0){
                sprintf(str1,"\n\rNew measurements made.\n\r",tCounter);
                measureTemperature();// Once every 6 min (10 times/hr)
                measureFlowRate(); // Once every 6 min (10 times/hr)
                if(measureAll){
                    measureSalinity(); //Once every 12 min (5 times/hr)
                    measureCarbon();// Once every 12 min (5 times/hr)
                }
                measureAll = ~measureAll;
                updateTerminal();
                terminalSendString(str1);
               
            }
            tCounter++;
            timerFlag = 0;
        }
        terminalTask();
        showMeasurements();
    }
}

