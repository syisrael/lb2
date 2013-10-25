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
void timer_isr (void);
void showMeasurements(void);

#pragma code high_vector=0x08
void high_interrupt (void) {_asm GOTO timer_isr _endasm}
#pragma code
#pragma interrupt timer_isr

void timer_isr (void)
{
	INTCONbits.TMR0IF = 0;
        tCounter++;
        if(tCounter % 90 == 0){
            closeLCD();
            measureTemperature();// Once every 6 min (10 times/hr)
            measureFlowRate(); // Once every 6 min (10 times/hr)
            if(measureAll){
                measureSalinity(); //Once every 12 min (5 times/hr)
                measureCarbon();// Once every 12 min (5 times/hr)
            }
            measureAll = ~measureAll;
        }
}

/*#pragma code high_vector=0x08
void low_interrupt (void) {_asm GOTO button_isr _endasm}
#pragma code
#pragma interrupt button_isr

void button_isr (void)
{


}*/

void showMeasurements(){
    int i;
    int value;
    openLCD();
    fahrenheit = PORTBbits.RB1; // F or C switch
    
    value = getTemp(fahrenheit);
    if(fahrenheit){
        if(value > 212){
            sprintf(str1,"Temp: >212F                \n\r");
        }else if(value < 23){
            sprintf(str1,"Temp: <23F                \n\r");
        }else{
            sprintf(str1,"Temp: %dF                \n\r",value);
        }
    }else{
        if(value > 100){
            sprintf(str1,"Temp: >100C                \n\r");
        }else if(value < -5){
            sprintf(str1,"Temp: <-5C                \n\r");
        }else{
            sprintf(str1,"Temp: %dC                \n\r",value);
        }
    }

    value = getSalinity();
    if(value > 50){
        sprintf(str2,"Salinity: >50ppt                \n\r");
    }else if(value < 5){
        sprintf(str2,"Salinity: <5ppt                \n\r");
    }else{
        sprintf(str2,"Salinity: %dppt                \n\r",getSalinity());
    }
    printLCD(str1,str2);
    for(i=0;i<5;i++)
        Delay10KTCYx(1000);

    value = getCarbon();
    if(value > 350){
        sprintf(str1,"Carbon: >350ppm                \n\r");
    }else if(value < 10){
        sprintf(str1,"Carbon: <10ppm                \n\r");
    }else{
        sprintf(str1,"Carbon: %dppm                \n\r",getCarbon());
    }
    value = getCarbon();
    if(value > 1000){
        sprintf(str2,"Flow: >1000dLps                \n\r");
    }else if(value < 100){
        sprintf(str2,"Flow: <100Lps                \n\r");
    }else{
        sprintf(str2,"Flow: %dLps                \n\r",getFlowRate());
    }
    printLCD(str1,str2);
    for(i=0;i<5;i++)
        Delay10KTCYx(1000);
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
    sprintf(str1,"EE478 Team6 Phase1");
    sprintf(str2,"Best Demo. EVER!");
    measureCarbon();
    measureTemperature();
    measureSalinity();
    measureFlowRate();
    updateTerminal();
    while(1){
        terminalTask();
        showMeasurements();
    }
}

