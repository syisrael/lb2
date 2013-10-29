#include <p18f452.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

void setupTimer(void) {
    OpenTimer0( TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_128);
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
#define     I2C_CLOCK_DIVIDER           9   // 400 kHZ

#define     MASTER_PACKET_SIZE          20
#define     SLAVE_PACKET_SIZE           52
#define     MASTER_BUFFER_SIZE          4*2 + 1
#define     SLAVE_BUFFER_SIZE           8*2 + 1

int retryAttempts = 0;
int errors = 0;
int referenceTimer = 0;
char transmitBuffer[MASTER_BUFFER_SIZE];
char receiveBuffer[SLAVE_BUFFER_SIZE];
char status = 0;
char buffer = 0;

typedef struct { char address, command, device, measureType; } I2CCommand;
I2CCommand cmd = { 0, 0, 0, 0 };

void setupI2C() {
    OpenI2C (MASTER, SLEW_OFF);
    SSPADD = I2C_CLOCK_DIVIDER;
    transmitBuffer[MASTER_PACKET_SIZE] = '\0';
    receiveBuffer[SLAVE_PACKET_SIZE] = '\0';
}

char i = 0;
void communications() {
    if (cmd.address > DEVICE_ADDRESS) {
        // Setup SLAVE<-MASTER
        IdleI2C();
        StartI2C();
        do {
            status = WriteI2C(cmd.address << 1); // Write

            if (status == -1) {
                getcI2C();
                SSPCON1bits.WCOL = 0b0;
            }
        } while (status != 0);
        // TX from MASTER->SLAVE
        for (i = 0; i < MASTER_BUFFER_SIZE; transmitBuffer[i++] = 0);
        transmitBuffer[0] = cmd.address;
        transmitBuffer[1] = cmd.command;
        transmitBuffer[2] = cmd.measureType;
        transmitBuffer[3] = cmd.device;
        transmitBuffer[4] = ~cmd.address;
        transmitBuffer[5] = ~cmd.command;
        transmitBuffer[6] = ~cmd.measureType;
        transmitBuffer[7] = ~cmd.device;
        while (putsI2C(transmitBuffer) != 0);

        // Setup MASTER<-SLAVE
        IdleI2C();
        RestartI2C();
        IdleI2C();
        getcI2C();
        do {
            status = WriteI2C((cmd.address << 1) | 0b1); // Read
            
            if (status == -1) {
                getcI2C();
                SSPCON1bits.WCOL = 0b0;
            }
        } while (status != 0);
        // RX from MASTER<-SLAVE
        do {
            while (getsI2C(receiveBuffer,SLAVE_PACKET_SIZE));
            (receiveBuffer[i] == )
            status = 0
        } while (status);
        NotAckI2C();
        while (SSPCON2bits.ACKEN != 0);
        StopI2C();
    }    
}

void main(void)
{
    setupTimer();
    setupTerminal();
    setupI2C();
    while(1) {
        if (flagTimer = 1) {
            communications();
            flagTimer = 0;
        }
    }
}