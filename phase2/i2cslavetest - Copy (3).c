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

void setupTimer(void);
void setupI2C(void);
void communications(void);

int fahrenheit = 1;
int tCounter = 0;
char flagTimer = 0, flagPrint = 0;
char damnit = 0, blah = 'a';

unsigned char rxBuffer[50];
unsigned short i = 0;
char x = '0';
char flagStart = 0;

#define PIN_X PORTBbits.RB0

#pragma code high_vector=0x08
void high_interrupt (void)
{
    INTCONbits.TMR0IF = 0;
    tCounter++;
    flagTimer = 1;
}
#pragma code
#pragma interrupt high_interrupt

void setupTimer() {
    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_128);
    INTCONbits.GIE = 1; //enable interrupts
    TRISBbits.RB1 = 1; //F or C switch
    //PIE1bits.SSPIE = 1; // Enable I2C interrupt
    //IPR1bits.SSPIP = 1; // High priority I2C interrupt
}

#define     MAX_RETRY_ATTEMPTS          10
#define     MAX_WAITTIME                5   // seconds
#define     DEVICE_ADDRESS              0xb0   // MASTER_ADDRESS 0
                                            // SLAVE0_ADDRESS 1
                                            // SLAVE1_ADDRESS 2
                                            // SLAVE2_ADDRESS 3
                                            // SLAVE3_ADDRESS 4
                                            // SLAVE4_ADDRESS 5
                                            // SLAVE5_ADDRESS 6
#define     I2C_CLOCK_DIVIDER           0x31   // 400 kHZ

#define     MASTER_BUFFER_SIZE          3*2 + 1
#define     SLAVE_BUFFER_SIZE           7*2 + 1

int retryAttempts = 0;
int errors = 0;
int referenceTimer = 0;
unsigned char transmitBuffer[SLAVE_BUFFER_SIZE];
unsigned char receiveBuffer[MASTER_BUFFER_SIZE];
short status = 0;

typedef struct { char address, command, device, measureType; } I2CCommand;
I2CCommand i2ccmd = { 0, 0, 0, 0 };

#define PIN_SCL_HZ TRISCbits.RC3
#define PIN_SDA_HZ TRISCbits.RC4

#define PIN_SCL PORTCbits.RC3
#define PIN_SDA PORTCbits.RC4

char str[20];

#define SCL     PORTCbits.RC3
#define SDA     PORTCbits.RC4
#define SCL_IN  TRISCbits.RC3
#define SDA_IN  TRISCbits.RC4

void I2CDelay() {
    Delay10TCYx(0x05);
}

void I2CStart()
{
  SDA = 1;
  I2CDelay();
  SCL = 1;
  I2CDelay();
  SDA = 0;
  I2CDelay();
  SCL = 0;
  I2CDelay();
}

void I2CStop(void)
{
  SDA = 0;             // i2c stop bit sequence
  I2CDelay();
  SCL = 1;
  I2CDelay();
  SDA = 1;
  I2CDelay();
}

unsigned char I2CGet(char ack)
{
    char x, d=0;
    SDA_IN = 1;
    for(x=0; x<8; x++) {
        d <<= 1;
        do {
            SCL_IN = 1;
        } while(SCL==0);    // wait for any SCL clock stretching
        I2CDelay();
        if(SDA)
            d |= 1;
        SCL_IN = 0;
    }
    if(ack) SDA = 0;
    else SDA = 1;
    SCL = 1;
    I2CDelay();             // send (N)ACK bit
    SCL = 0;
    SDA = 1;
    return d;
}

unsigned char I2CPut(unsigned char d)
{
    char x;
    unsigned char b;
    for(x=8; x; x--) {
        if(d&0x80) SDA = 1;
        else SDA = 0;
        I2CDelay();
        SCL = 1;
        d <<= 1;
        I2CDelay();
        SCL = 0;
    }
    SDA = 1;
    SCL = 1;
    I2CDelay();
    b = SDA_IN;          // possible ACK bit
    SCL = 0;
    return b;
}

char data = 'a', dump;
void communications() {
    //PIN_X = ~PIN_X;
    dump = I2CGet(1);

    putcUSART(dump);
}

char asdf = '0';
void main(void)
{
    TRISBbits.RB0 = 0;
    setupTimer();
    setupTerminal();
    terminalSendPString("\033[2J");
    while (BusyUSART());
    putrsUSART("running\r\n");
    while(1) {
        communications();
        if (flagPrint) {
            terminalSendString(rxBuffer);
            flagPrint = 0;
        }
        if (flagTimer) {
            while (BusyUSART());
            putcUSART(asdf++);
            flagTimer = 0;
            if (asdf > '9') {
                asdf = '0';
                terminalSendPString("\r\n");
            }
        }
    }
}