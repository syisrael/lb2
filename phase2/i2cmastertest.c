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

void setupTimer(void);
void setupI2C(void);
void communications(void);

int tCounter = 0;
char flagTimer = 0;
void timer_isr (void)
{
	INTCONbits.TMR0IF = 0;
	tCounter++;
        flagTimer = 1;
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
                                            // SLAVE0_ADDRESS 1
                                            // SLAVE1_ADDRESS 2
                                            // SLAVE2_ADDRESS 3
                                            // SLAVE3_ADDRESS 4
                                            // SLAVE4_ADDRESS 5
                                            // SLAVE5_ADDRESS 6
#define     I2C_CLOCK_DIVIDER           0x31   // 400 kHZ

#define     MASTER_BUFFER_SIZE          3*2 + 1
#define     SLAVE_BUFFER_SIZE           8*2 + 1

int retryAttempts = 0;
int errors = 0;
int referenceTimer = 0;
unsigned char transmitBuffer[MASTER_BUFFER_SIZE];
unsigned char receiveBuffer[SLAVE_BUFFER_SIZE];
short status = 0;

typedef struct { char address, command, device, measureType; } I2CCommand;
I2CCommand i2ccmd = { 0, 0, 0, 0 };

void setupI2C() {
    TRISCbits.RC3 = 0;
    TRISCbits.RC4 = 0;
    OpenI2C (MASTER, SLEW_OFF);
    SSPADD = I2C_CLOCK_DIVIDER;
    transmitBuffer[MASTER_BUFFER_SIZE] = '\0';
    receiveBuffer[SLAVE_BUFFER_SIZE] = '\0';
}

char i = 0;
void communications() {
    if (i2ccmd.address > DEVICE_ADDRESS) {
        // Setup MASTER->SLAVE
        CloseI2C();
        IdleI2C();
        StartI2C();

        IdleI2C();
        do {
            status = putcI2C(i2ccmd.address << 1); // Write

            if (status == -1) {
                getcI2C();
                SSPCON1bits.WCOL = 0b0;
            }
        } while (status != 0);
        // TX from MASTER->SLAVE
        for (i = 0; i < MASTER_BUFFER_SIZE; transmitBuffer[i++] = 0);
        transmitBuffer[0] = i2ccmd.command;
        transmitBuffer[1] = i2ccmd.measureType;
        transmitBuffer[2] = i2ccmd.device;
        transmitBuffer[3] = ~i2ccmd.command;
        transmitBuffer[4] = ~i2ccmd.measureType;
        transmitBuffer[5] = ~i2ccmd.device;
        while (putsI2C(transmitBuffer) != 0);

        // Setup MASTER<-SLAVE
        IdleI2C();
        RestartI2C();
        IdleI2C();
        getcI2C();
        do {
            status = WriteI2C((i2ccmd.address << 1) | 0b1); // Read
            
            if (status == -1) {
                getcI2C();
                SSPCON1bits.WCOL = 0b0;
            }
        } while (status != 0);
        // RX from MASTER<-SLAVE
        do {
            status = 0;
            while (getsI2C(receiveBuffer, SLAVE_BUFFER_SIZE));
            for (i = 0; i < SLAVE_BUFFER_SIZE / 2; i++) {
                status |= (receiveBuffer[i] == ~receiveBuffer[SLAVE_BUFFER_SIZE / 2 + i]) << i;
            }
        } while (status != 0xffff);
        NotAckI2C();
        while (SSPCON2bits.ACKEN != 0);
        StopI2C();

        //Handle valid data in receiveBuffer 0-15
    }    
}

void main(void)
{
    setupTimer();
    //setupTerminal();
    setupI2C();
    while(1) {
        if (flagTimer) {
            i2ccmd.address = 1;
            i2ccmd.command = 1;
            i2ccmd.device = 1;
            i2ccmd.measureType = 1;
            communications();
            flagTimer = 0;
        }
    }
}