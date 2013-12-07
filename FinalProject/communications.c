/* Compile options:  -ml (Large code model) */
#include <p18f452.h>
#include <usart.h>
#include <timers.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <timers.h>
#include <delays.h>
#include "communications.h"
#include "movement.h"
#include "lcd.h"
#include "lcd/xlcd.h"

#define ACK 0x06
#define NAK 0x15

void Ack();

char cmdType;
char cmd[50];
char screenbuff1[30];
char screenbuff2[30];
char * ptr;
char dir;
int row;
int* wrow;
extern int nrow0,nrow1,nrow2,nrow3,nrow4,nrow5,nrow6,nrow7;
extern int r0count[12],r1count[12],r2count[12],r3count[12],r4count[12],r5count[12],r6count[12],r7count[12];
extern char gameSettings;

void sendPString(const char* str)
{
    while(BusyUSART());
    putrsUSART(str);
}

void sendString(char *str)
{
    while(BusyUSART());
    putsUSART(str);
}


void usartTask()
{
    int i;
    while(BusyUSART());
    if (DataRdyUSART()) {
        cmdType = getcUSART();
    }

    switch (cmdType) {
    case 0x1D: //Move To UP/DOWN/LEFT/RIGHT/DIAGONAL
        Ack();
        while(!DataRdyUSART());
        dir = getcUSART();
        Ack();
        setupLCD();
        sprintf(screenbuff1,"To: %d, %c",(int)dir,dir);
        printLCD(screenbuff1,screenbuff2);
        move((int)dir);
        break;
     case 0x02: //Show message on LCD
        Ack();
        while(BusyUSART());
        getsUSART(cmd,32);
        Ack();
        setupLCD();
        strncpy(screenbuff1,cmd,16);
        strncpy(screenbuff2,&cmd[16],16);
        printLCD(screenbuff1,screenbuff2);
        break;
    case 0x03: //Extend magnets
        Ack();
        extendSolenoid();
        break;
    case 0x11: //Retract magnets
        Ack();
        retractSolenoid();
        break;
    case 0x12: //Sensor reset
        Ack();
        saveBackup();
        break;
    case 0x13: //Request sensor data
        Ack();
        row = nrow0;
        while(BusyUSART());
        putcUSART(((char*)&row)[0]);
        while(BusyUSART());
        putcUSART(((char*)&row)[1]);
        row = nrow1;
        while(BusyUSART());
        putcUSART(((char*)&row)[0]);
        while(BusyUSART());
        putcUSART(((char*)&row)[1]);
        row = nrow2;
        while(BusyUSART());
        putcUSART(((char*)&row)[0]);
        while(BusyUSART());
        putcUSART(((char*)&row)[1]);
        row = nrow3;
        while(BusyUSART());
        putcUSART(((char*)&row)[0]);
        while(BusyUSART());
        putcUSART(((char*)&row)[1]);
        row = nrow4;
        while(BusyUSART());
        putcUSART(((char*)&row)[0]);
        while(BusyUSART());
        putcUSART(((char*)&row)[1]);
        row = nrow5;
        while(BusyUSART());
        putcUSART(((char*)&row)[0]);
        while(BusyUSART());
        putcUSART(((char*)&row)[1]);
        row = nrow6;
        while(BusyUSART());
        putcUSART(((char*)&row)[0]);
        while(BusyUSART());
        putcUSART(((char*)&row)[1]);
        row = nrow7;
        while(BusyUSART());
        putcUSART(((char*)&row)[0]);
        while(BusyUSART());
        putcUSART(((char*)&row)[1]);
        break;
    case 0x14: //Request sensor count
        Ack();
        wrow = r0count;
        for(i=0;i<12;i++){
            row = wrow[i];
            while(BusyUSART());
            putcUSART(((char*)&row)[0]);
        }
        
        wrow = r1count;
        for(i=0;i<12;i++){
            row = wrow[i];
            while(BusyUSART());
            putcUSART(((char*)&row)[0]);
        }
        
        wrow = r2count;
        for(i=0;i<12;i++){
            row = wrow[i];
            while(BusyUSART());
            putcUSART(((char*)&row)[0]);
        }
        
        wrow = r3count;
        for(i=0;i<12;i++){
            row = wrow[i];
            while(BusyUSART());
            putcUSART(((char*)&row)[0]);
        }
        
        wrow = r4count;
        for(i=0;i<12;i++){
            row = wrow[i];
            while(BusyUSART());
            putcUSART(((char*)&row)[0]);
        }
        
        wrow = r5count;
        for(i=0;i<12;i++){
            row = wrow[i];
            while(BusyUSART());
            putcUSART(((char*)&row)[0]);
        }
        
        wrow = r6count;
        for(i=0;i<12;i++){
            row = wrow[i];
            while(BusyUSART());
            putcUSART(((char*)&row)[0]);
        }

        wrow = r7count;
        for(i=0;i<12;i++){
            row = wrow[i];
            while(BusyUSART());
            putcUSART(((char*)&row)[0]);
        }
        break;
    case 0x1C: //Request game settings
        Ack();
        while(BusyUSART());
        putcUSART(gameSettings);
        break;
    default:
        break;
    }

    for(i=0;i<50;i++){
        cmd[i] = ' ';
    }
    cmd[49] = '\0';
    cmdType = 0xff;
}

void Ack(){
    while(BusyUSART());
    putcUSART(ACK);
}

void setupCommunications()
{
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &
              USART_EIGHT_BIT & USART_CONT_RX, 129);
}
