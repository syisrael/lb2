/* */
//#include <p18f452.h>
//#include <p18f452.h>
#include <p18f4620.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <timers.h>
#include "communications.h"
#include <i2c.h>
#include <usart.h>

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

int fahrenheit = 1;
int tCounter = 0;
char flagTimer = 0, flagPrint = 0;
char damnit = 0, blah = 'a';

unsigned char rxBuffer[50];
unsigned char txBuffer[50] = "zxcvlol";
unsigned short i = 0;
unsigned short j = 0;
char x = '0';
char flagStart = 0;

#define PIN_X PORTBbits.RB2

void high_isr (void)
{
    if(PIR1bits.SSPIF == 1) //if MSSP interrupt
    {
        SSPCON1bits.CKP = 1;
        if (SSPSTATbits.S && SSPSTATbits.BF) {
            i = SSPBUF;
            if (SSPSTATbits.D_NOT_A) {
                if (SSPSTATbits.R_NOT_W) {
                    
                    for (j=0;j<8;j++) {
                        SSPCON1bits.CKP = 0;
                        SSPBUF = txBuffer[j];
                        SSPCON1bits.CKP = 1;
                    }
                    
                } else {
                    rxBuffer[i++] = SSPBUF;
                }
            } else {
                i = 0;
            }
            flagStart = 1;
        }
        /*if (SSPSTATbits.S && SSPSTATbits.BF && !SSPSTATbits.D_NOT_A) {
            PIN_X = ~PIN_X;
            x = SSPBUF;
            putcUSART('o');
            i = 0;
        } else if (SSPSTATbits.BF && SSPSTATbits.D_NOT_A && !SSPSTATbits.R_NOT_W) { // Reset pointer
            x = SSPBUF;
            putcUSART('x');
            rxBuffer[i++] = '\0';
        } else if (SSPSTATbits.BF && !SSPSTATbits.D_NOT_A && SSPSTATbits.R_NOT_W) { // Reset pointer
            x = SSPBUF;
            putcUSART('r');
            rxBuffer[i++] = '\0';
        } else if (SSPSTATbits.P && flagStart) { // Reset pointer
//            ReadI2C();
            putcUSART(']');
            rxBuffer[i] = '\0';
            flagPrint = 1;
            flagStart = 0;
        }*/
        PIR1bits.SSPIF = 0; //reset MSSP interrupt flag bit
    }
    /*if (INTCONbits.TMR0IF == 1) {
        INTCONbits.TMR0IF = 0;
        tCounter++;
        flagTimer = 1;
    }*/
}

#pragma code high_vector=0x08
void high_interrupt (void)
{
	_asm GOTO high_isr _endasm
}
#pragma code
#pragma interrupt high_isr

void setupTimer() {
    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_128);
    INTCONbits.GIE = 1; //enable interrupts
    TRISBbits.RB1 = 1; //F or C switch
    PIE1bits.SSPIE = 1; // Enable I2C interrupt
    IPR1bits.SSPIP = 1; // High priority I2C interrupt
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

void setupI2C() {
    OpenI2C(SLAVE_7_STSP_INT, SLEW_OFF);
    SSPADD = 0xb0;
}

char str[20];
char data = 'a', dump;
void communications() {
    //PIN_X = ~PIN_X;
    //dump = I2CGet(1);

    /*if (DataRdyI2C()) {
        x = SSPBUF;

    }*/
}

char asdf = '0';
void main(void)
{
    TRISB = 0x00;
    PIN_X = 0;
    setupTimer();
    setupTerminal();
    setupI2C();
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