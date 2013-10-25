#include <p18f452.h>
#include <usart.h>
#include <delays.h>
#include <timers.h>
#include "lcd/xlcd.h"
#include "lcd.h"

#include <stdio.h>
#include <stdlib.h>

extern char flagDisplayOn;

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