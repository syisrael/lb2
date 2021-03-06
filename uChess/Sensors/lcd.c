#include <p18f452.h>
#include <delays.h>
#include <timers.h>
#include <string.h>
#include "xlcd.h"
#include "lcd.h"

char flagDisplayOn = 0;

// Delay for 18 ms
void DelayFor18TCY(void) {
    Delay1KTCYx(1);
}

// Delay for 15 ms
void DelayPORXLCD(void) {
    Delay1KTCYx(1);
}

// Delay for 5 ms
void DelayXLCD(void) {
    Delay1KTCYx(1);
}

void setupLCD(void) {
    TRISDbits.RD7 = 0;
    OpenXLCD(FOUR_BIT & LINES_5X7);
    WriteCmdXLCD(SHIFT_DISP_LEFT);
    clearLCD();
}

void openLCD(void) {
    if (!flagDisplayOn) {
        WriteCmdXLCD(DON);
        while (BusyXLCD());
        flagDisplayOn = 1;
        PORTDbits.RD7 = 0;
    }
}

void closeLCD(void) {
    WriteCmdXLCD(DOFF);
    while (BusyXLCD());
    flagDisplayOn = 0;
    PORTDbits.RD7 = 1;
}

void clearLCD(void) {
    openLCD();
    while (BusyXLCD());
    SetDDRamAddr(0x80);
    while (BusyXLCD());
    putrsXLCD("                ");
    while (BusyXLCD());
    SetDDRamAddr(0xc0);
    putrsXLCD("                ");
}

void printLCD(char *line1, char *line2) {
    int i;
    if (strlen(line1)<16){
        for(i=strlen(line1);i <16;i++)
            line1[i] = ' ';
    }
    if (strlen(line2)<16){
        for(i=strlen(line2);i <16;i++)
            line2[i] = ' ';
    }
    line1[16] = '\0';
    line2[16] = '\0';
    
    openLCD();
    while (BusyXLCD());
    SetDDRamAddr(0x80);
    while (BusyXLCD());
    putsXLCD(line1);
    while (BusyXLCD());
    SetDDRamAddr(0xc0);
    while (BusyXLCD());
    putsXLCD(line2);
}