/* */
#include <p18f452.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <timers.h>
#include "communications.h"
#include <i2c.h>
#include <usart.h>

#pragma config WDT=OFF              // Watchdog off
#pragma config BOR=OFF              // Brown out reset
#pragma config LVP=OFF              //
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
#pragma config CP2=OFF              // Code protection
#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection

void setupI2C(void);
void communications(void);

void setupI2C() {
    OpenI2C (SLAVE_7, SLEW_OFF);
    SSPCON1bits.CKP = 1;
    SSPADD = 0xB0;
}

void main(void)
{
    short addr, data;
    setupI2C();
    while(1){
        if( DataRdyI2C() )
        {
            addr = ReadI2C();
            AckI2C();
            IdleI2C();
            data = ReadI2C();
            putcUSART(data);
            NotAckI2C();
            IdleI2C();
        }
    }
}