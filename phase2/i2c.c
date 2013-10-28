#include <p18f452.h>
#include <i2c.h>
#include <referenceTimers.h>
#include <ctype.h>
#include <stdio.h>
#include <referenceTimers.h>

#define     MAX_RETRY_ATTEMPTS          10
#define     MAX_WAITTIME                5       // seconds

#define     MASTER_ADDRESS              0
#define     SLAVE0_ADDRESS              1
#define     SLAVE1_ADDRESS              2
#define     SLAVE2_ADDRESS              3
#define     SLAVE3_ADDRESS              4
#define     SLAVE4_ADDRESS              5
#define     SLAVE5_ADDRESS              6

#define     DEVICE_ADDRESS              0   // MASTER_ADDRESS 0
                                            // SLAVE0_ADDRESS 1
                                            // SLAVE1_ADDRESS 2
                                            // SLAVE2_ADDRESS 3
                                            // SLAVE3_ADDRESS 4
                                            // SLAVE4_ADDRESS 5
                                            // SLAVE5_ADDRESS 6
#define     I2C_CLOCK_DIVIDER           9   //
                                        
// flagTXData
// flagRXData
// retryAttempts
// errors
// referenceTimer

#if !DEVICE_ADDRESS
// MASTER
typedef enum {M_IDLE, M_TRANSMIT, M_WAITACK, M_LISTEN, M_RECEIVE, M_ERROR} MasterState;
MasterState mState = M_IDLE;

void setupI2C() {
    OpenI2C (MASTER, SLEW_OFF);
    SSPADD = I2C_CLOCK_DIVIDER;
    StartI2C();
}

void communications() {
    switch (mState) {
    case M_IDLE:
        if (flagTXData) {
            mState = M_TRANSMIT;
            retryAttempts = 0;
        }
        break;
    case M_TRANSMIT:
        // send TXData on bus
        retryAttempts++;
        referenceTimer = 0;
        mState = M_WAITACK;
        break;
    case M_WAITACK:
        if (/* receive ack */) { // Slave ACK
            mState = M_LISTEN;
            retryAttempts = 0;
        } else if (retryAttempts > MAX_RETRY_ATTEMPTS) { // Limit exceeded
            mState = M_ERROR;
        } else if (referenceTimer > MAX_WAITTIME) { // Retransmit
            mState = M_TRANSMIT;
        }
        break;
    case M_LISTEN:
        if (retryAttempts > MAX_RETRY_ATTEMPTS | referenceTimer > MAX_WAITTIME) { // Limit exceeded
            mState = M_ERROR;
        } else if (flagRXData) { // Data is on bus
            referenceTimer = hwTimer;
            mState = M_RECEIVE;
        }
        break;
    case M_RECEIVE:
        // receive data on bus
        if (hwTimer - referenceTimer > MAX_WAITTIME) { // Limit exceeded
            mState = M_ERROR;
        } else if (/* error data */) { // Error with data, make slave retransmit
            retryAttempts++;
            mState = M_LISTEN;
        } else if (/* data is good */) {
            AckI2C(); // Send ACK
            mState = M_IDLE;
        }
        break;
    case M_ERROR:
        errors++;
        mState = M_IDLE;
        break;
    }
}
#else
// SLAVE
typedef enum {S_LISTEN, S_PROCESSBUS, S_TRANSMIT, S_WAITACK, S_ERROR} SlaveState;
SlaveState sState = S_LISTEN;

void setupI2C() {
    OpenI2C (SLAVE, SLEW_OFF);
    SSPADD = I2C_CLOCK_DIVIDER;
    StartI2C();
}

void communications() {
    switch (sState) {
    case S_LISTEN:
        if (flagRXData) {
            sState = S_PROCESSBUS;
        }
        break;
    case S_PROCESSBUS:
        // receive RXData on bus
        if (/* data is good */ & /* command is addressed to me */) {
            // send ACK to master
            sState = S_TRANSMIT;
            retryAttempts = 0;
        } else {
            sState = S_LISTEN;
        }
        break;
    case S_TRANSMIT:
        // send TXData on bus
        retryAttempts++;
        referenceTimer = hwTimer;
        sState = S_WAITACK;
        break;
    case S_WAITACK:
        if (/* receive ack */) { // Master ACK
            AckI2C(); // Send ACK
            sState = S_LISTEN;
        } else if (retryAttempts > MAX_RETRY_ATTEMPTS) { // Limit exceeded
            sState = S_ERROR;
        } else if (hwTimer - referenceTimer > MAX_WAITTIME) { // Retransmit
            sState = S_TRANSMIT;
        }
        break;
    case S_ERROR:
        errors++;
        sState = S_LISTEN;
        break;
    }
}
#endif