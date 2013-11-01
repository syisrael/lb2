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
void high_isr (void)
{
    /*if(PIR1bits.SSPIF == 1) //if MSSP interrupt
    {
        if (SSPSTATbits.S && SSPSTATbits.BF) { // Reset pointer
            if (SSPSTATbits.D_A) {
                x = SSPBUF;
                rxBuffer[i++] = x;
                putcUSART('x');
            } else {
                ReadI2C();
                //damnit = SSPBUF;
                putcUSART('[');
                flagStart = 1;
                i = 0;
            }
        } else if (SSPSTATbits.P && flagStart) { // Reset pointer
//            ReadI2C();
            putcUSART(']');
            rxBuffer[i] = '\0';
            flagPrint = 1;
            flagStart = 0;
        }
        PIR1bits.SSPIF = 0; //reset MSSP interrupt flag bit
    }/**/
    if (INTCONbits.TMR0IF == 1) {
        INTCONbits.TMR0IF = 0;
        tCounter++;
        flagTimer = 1;
    }
}

#pragma code high_vector=0x08
void high_interrupt (void)
{
	_asm GOTO high_isr _endasm
}
#pragma code
#pragma interrupt high_isr

void setupTimer() {
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_128);
    INTCONbits.GIE = 1; //enable interrupts
    TRISBbits.RB1 = 1; //F or C switch
    //PIE1bits.SSPIE = 1; // Enable I2C interrupt
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

void setupI2C() {
    PIN_SCL_HZ = 1;
    PIN_SDA_HZ = 1;

    //OpenI2C (SLAVE_7, SLEW_OFF);

    SSPCON1 = 0b00110110;  // 7-bit Slave mode, START/STOP interrupts enabled, SSPEN enabled (serial port)
    SSPCON2 = 0b00000000;   // Clock stretching is enabled for slave transmit only
    SSPSTATbits.SMP = 1;    // Slew rate 100 kHz
    SSPSTATbits.CKE = 1;    // Enable SMBus specific inputs
    //SSPSTATbits.CKE = 0;    // Disable SMBus specific inputs
    SSPCON1bits.CKP = 1;        // Release clock

    SSPADD = 0xb0;
    transmitBuffer[SLAVE_BUFFER_SIZE] = '\0';
    receiveBuffer[MASTER_BUFFER_SIZE] = '\0';
}

/*char asdf = 'a';
char str[80];
void main(void)
{
    
    setupTimer();
    setupTerminal();
    setupI2C();
    terminalSendPString("\033[2J");
    while (BusyUSART());
    putrsUSART("running\r\n");
    while(1) {
        if (flagPrint) {
            //sprintf(str, "[%s]", rxBuffer);
            terminalSendPString("\r\n");
            terminalSendString(rxBuffer);
            terminalSendPString("\r\n");
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

/*void setupI2C() {
    OpenI2C (SLAVE_7, SLEW_OFF);
    SSPCON1 |= 16;
    //SSPCON1bits.CKP = 1;
    SSPADD = DEVICE_ADDRESS;
    transmitBuffer[SLAVE_BUFFER_SIZE] = '\0';
    receiveBuffer[MASTER_BUFFER_SIZE] = '\0';
}*/

char str[20];
char addr, data = 'q';
void communications() {
    // Setup SLAVE<-MASTER

    if (DataRdyI2C()) {
        data = SSPBUF;
        putcUSART('g');
        if (SSPSTATbits.R_NOT_W) {
            putcUSART('R');
        } else {
            putcUSART('W');
        }
    }
}

char asdf = '0';
void main(void)
{

    setupTimer();
    setupTerminal();
    setupI2C();
    terminalSendPString("\033[2J");
    while (BusyUSART());
    putrsUSART("running\r\n");
    while(1) {
        communications();
        if (flagPrint) {
            //sprintf(str, "[%s]", rxBuffer);
            terminalSendPString("\r\n");
            terminalSendString(rxBuffer);
            terminalSendPString("\r\n");
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

    /*if (DataRdyI2C()) {
        addr = ReadI2C();
        AckI2C();
        IdleI2C();
        data = ReadI2C();
        NAckI2C();
        IdleI2C();
        /*getcI2C();
        AckI2C();
        IdleI2C();
        getsI2C(receiveBuffer, MASTER_BUFFER_SIZE);
        IdleI2C();
        AckI2C();
        sprintf(str, "%s", receiveBuffer);
        terminalSendPString("Printing contents:\r\n");
        terminalSendString(str);
        terminalSendPString("Done\r\n");
        IdleI2C();
        NotAckI2C();
        IdleI2C();*/
        /*getsI2C(receiveBuffer, MASTER_BUFFER_SIZE);
        for (i = 0; i < MASTER_BUFFER_SIZE / 2; i++) {
            status |= (receiveBuffer[i] == ~receiveBuffer[MASTER_BUFFER_SIZE / 2 + i]) << i;
        }
        terminalSendPString("Some Bit Thing\n\r");
        terminalSendPString("Listening Again\n\r");
        while (!DataRdyI2C());
        getcI2C();
        terminalSendPString("Receiving Buffer\n\r");
        if (SSPSTAT & 0x04) {
            while (putsI2C(transmitBuffer));
        }
        terminalSendPString("Print Contents To USART\n\r");*/
    //}
/*}*/