//#include <p18f452.h>
#include <p18f4585.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <timers.h>
#include <delays.h>

#include <i2c.h>

#pragma config WDT=OFF              // Watchdog off
//#pragma config BOR=OFF              // Brown out reset
#pragma config LVP=OFF              //
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
#pragma config CP2=OFF              // Code protection
//#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection

void setupTimer(void);
void setupI2C(void);
void communications(void);
void DelayTCY(unsigned char);
void Delay10TCYx(unsigned char);
void Delay100TCY(unsigned char);
void Delay1KTCYx(unsigned char);
void Delay10KTCY(unsigned char);
unsigned char I2CPut(unsigned char);
unsigned char I2CGet(char);

int tCounter = 0;
char flagTimer = 0;
char x;
void timer_isr (void)
{
    if (PIR1bits.SSPIF) {
        if (SSPSTATbits.BF) {
            if (SSPSTATbits.R_NOT_W) {
                x = SSPBUF;
            }
        }

        PIR1bits.SSPIF = 0;
    }

    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;
        tCounter++;
        flagTimer = 1;
    }
}

#pragma code high_vector=0x08
void high_interrupt (void)
{
	_asm GOTO timer_isr _endasm
}
#pragma code
#pragma interrupt timer_isr

void setupTimer() {
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_128);
    INTCONbits.GIE = 1; //enable interrupts
    TRISBbits.RB1 = 1; //F or C switch
}

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

int retryAttempts = 0;
int errors = 0;
int referenceTimer = 0;
unsigned char transmitBuffer[SLAVE_BUFFER_SIZE];
unsigned char receiveBuffer[SLAVE_BUFFER_SIZE];
short status = 0;

typedef struct { char address, command, measureType, fetchCount; } I2CCommand;
I2CCommand i2ccmd = { 0, 0, 0, 0 };

char b = 'a';

void communications() {
    StartI2C();
    IdleI2C();
    WriteI2C(0xb0);
    IdleI2C();
    WriteI2C(b++);
    if (b > 'z') b = 'a';
    IdleI2C();
    StopI2C();
    IdleI2C();

    StartI2C();
    IdleI2C();
    WriteI2C(0xb1);
    IdleI2C();
    b = ReadI2C();
    b--;
    IdleI2C();
    NotAckI2C();
    IdleI2C();
    StopI2C();
    IdleI2C();

    Delay1KTCYx(1);
}

void main(void)
{
    i2ccmd.address = 0xb0;
    i2ccmd.command = 'U';
    i2ccmd.measureType = 1;
    i2ccmd.fetchCount = 1;
    setupTimer();
    //setupTerminal();
    receiveBuffer[3] = '\0';
    PIE1bits.SSPIE = 1;
    PIR1bits.SSPIF = 1;

    OpenI2C(MASTER, SLEW_OFF);
    SSPADD = 0x09;

    while(1) {
        communications();
        if (flagTimer) {
            //communications();
            flagTimer = 0;
        }
    }
}