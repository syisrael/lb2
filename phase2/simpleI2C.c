#include <p18f452.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <timers.h>

#include <i2c.h>

#pragma config WDT=OFF              // Watchdog off
#pragma config BOR=OFF              // Brown out reset
#pragma config LVP=OFF              //
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
#pragma config CP2=OFF              // Code protection
#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection
#pragma config OSC = XT     

#define     MAX_RETRY_ATTEMPTS          10
#define     MAX_WAITTIME                5   // seconds
#define     DEVICE_ADDRESS              0   // MASTER_ADDRESS 0
                                            // SLAVE0_ADDRESS A1
                                            // SLAVE1_ADDRESS A2
                                            // SLAVE2_ADDRESS A3
                                            // SLAVE3_ADDRESS A4
                                            // SLAVE4_ADDRESS A5
                                            // SLAVE5_ADDRESS A6
#define     Fosc                        20000000
#define     Fi2c                        100000
#define     I2C_CLOCK_DIVIDER           (Fosc/Fi2c)/4-1   // (Fosc/Fi2c)/4-1

#define     MASTER_BUFFER_SIZE          3*2 + 1
#define     SLAVE_BUFFER_SIZE           8*2 + 1

void setupI2C(void);
void communications(void);

void setupI2C() {
    OpenI2C (MASTER, SLEW_ON);
    SSPADD = 0x31;
}

void communications() {
        StartI2C();
        IdleI2C();
        putcI2C(0xa1);
        IdleI2C();
        putcI2C(0x22);
        IdleI2C();
        StopI2C();
}

void main(void)
{
    setupI2C();
    while(1) {
        communications();
    }
}