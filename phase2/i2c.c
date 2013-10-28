#include <p18f452.h>
#include <i2c.h>
#include <timers.h>
#include <ctype.h>
#include <stdio.h>
#include <timers.h>

#define     MAX_RETRY_ATTEMPTS          10
#define     MAX_WAITTIME                0xffff

#define     MASTER_ADDRESS              0
#define     SLAVE0_ADDRESS              1
#define     SLAVE1_ADDRESS              2
#define     SLAVE2_ADDRESS              3
#define     SLAVE3_ADDRESS              4
#define     SLAVE4_ADDRESS              5
#define     SLAVE5_ADDRESS              6

typedef enum {M_IDLE, M_TRANSMIT, M_WAITACK, M_LISTEN, M_RECEIVE, M_ERROR} MasterState;
typedef enum {S_LISTEN, S_PROCESSBUS, S_TRANSMIT, S_WAITACK, S_ERROR} SlaveState;

MasterState mState = M_IDLE;
SlaveState sState = S_LISTEN;
// flagTXData
// flagRXData
// retryAttempts
// errors
// timer

void masterCommunications() {
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
        timer = 0;
        mState = M_WAITACK;
        break;
    case M_WAITACK:
        if (/* receive ack */) { // Slave ACK
            mState = M_LISTEN;
            retryAttempts = 0;
        } else if (retryAttempts > MAX_RETRY_ATTEMPTS) { // Limit exceeded
            mState = M_ERROR;
        } else if (timer > MAX_WAITTIME) { // Retransmit
            mState = M_TRANSMIT;
        } else { // Wait
            timer++;
        }
        break;
    case M_LISTEN:
        if (retryAttempts > MAX_RETRY_ATTEMPTS | timer > MAX_WAITTIME) { // Limit exceeded
            mState = M_ERROR;
        } else if (flagRXData) { // Data is on bus
            timer = 0;
            mState = M_RECEIVE;
        } else { // Wait
            timer++;
        }
        break;
    case M_RECEIVE:
        // receive data on bus
        if (timer > MAX_WAITTIME) { // Limit exceeded
            mState = M_ERROR;
        } else if (/* error data */) { // Error with data, make slave retransmit
            retryAttempts++;
            mState = M_LISTEN;
        } else if (/* data is good */) {
            // send ACK
            mState = M_IDLE;
        } else { // Wait
            timer++;
        }
        break;
    case M_ERROR:
        errors++;
        mState = M_IDLE;
        break;
    }
}

void slaveCommunications() {
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
        timer = 0;
        sState = S_WAITACK;
        break;
    case S_WAITACK:
        if (/* receive ack */) { // Master ACK
            // sendACK
            sState = S_LISTEN;
        } else if (retryAttempts > MAX_RETRY_ATTEMPTS) { // Limit exceeded
            sState = S_ERROR;
        } else if (timer > MAX_WAITTIME) { // Retransmit
            sState = S_TRANSMIT;
        } else { // Wait
            timer++;
        }
        break;
    case S_ERROR:
        errors++;
        sState = S_LISTEN;
        break;
    }
}