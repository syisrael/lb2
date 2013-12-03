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

char cmdType;
char cmd[50];
char screenbuff1[16] = "               ";
char screenbuff2[16] = "               ";
char * ptr;
long x,y;
int row;

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
        if (DataRdyUSART()) {
            while(BusyUSART());
            cmdType = getcUSART();
            putcUSART(0x06);
        }

        switch (cmdType) {
        case 0x01: //Move To (x,y)
            while(BusyUSART());
            getsUSART(&x,4);
            while(BusyUSART());
            getsUSART(&y,4);
            sprintf(screenbuff1,"To: %d,%d",(int)x,(int)y);
            printLCD(screenbuff1,screenbuff2);
            moveTo(x,y);
            break;
         case 0x02: //Show message on LCD
            while(BusyUSART());
            getsUSART(cmd,32);
            strncpy(screenbuff1,cmd,16);
            strncpy(screenbuff2,&cmd[16],16);
            printLCD(screenbuff1,screenbuff2);
            break;
        case 0x03: //Extend magnets
            extendSolenoid();
            break;
        case 0x11: //Retract magnets
            retractSolenoid();
            break;
        case 0x12: //Return to home
            resetPosition();
            break;
        case 0x13: //Request sensor data
            enableRead();
            Delay10TCYx(1);
            for(i=0;i<8;i++){
                row = readSensors();
                putcUSART(((char*)&row)[0]);
                putcUSART(((char*)&row)[1]);
                putcUSART(((char*)&row)[2]);
                Delay10TCYx(1);
            }
            disableRead();
            break;
        case 0x14: //Request sensor count
                    
            break;
        case 0xff:
            break;
        }

        for(i=0;i<50;i++){
            cmd[i] = ' ';
        }
        cmd[49] = '\0';
        cmdType = 0xff;
}



void setupCommunications()
{
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &
              USART_EIGHT_BIT & USART_CONT_RX, 129);
}
