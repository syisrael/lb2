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
#include "lcd.h"
#include "lcd/xlcd.h"

char cmdType;
char cmd[50];
char screenbuff1[16] = "               ";
char screenbuff2[16] = "               ";
char * ptr;
int x,y;
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
            putcUSART(cmdType);
        }

        switch (cmdType) {
        case 'a': //Move To (x,y)
            while(BusyUSART());
            getsUSART(&x,2);
            while(BusyUSART());
            getsUSART(&y,2);
            sprintf(screenbuff1,"To: %d,%d",x,y);
            sendString(screenbuff1);
            printLCD(screenbuff1,screenbuff2);
            break;
         case 'b': //Show message on LCD
            while(BusyUSART());
            getsUSART(cmd,32);
            strncpy(screenbuff1,cmd,16);
            strncpy(screenbuff2,&cmd[16],16);
            printLCD(screenbuff1,screenbuff2);
            break;
        case 'c': //Extend magnets

            break;
        case 'd': //Retract magnets

            break;
        case 'e': //Request sensor data
            enableRead();
            Delay10TCYx(1);
            for(i=0;i<8;i++){
                row = readSensors();
                putcUSART(((char*)&row)[0]);
                putcUSART(((char*)&row)[1]);
                Delay10TCYx(1);
            }
            disableRead();
            break;
        case 'f': //Return to home
            
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
