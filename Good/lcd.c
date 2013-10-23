/* Compile options:  -ml (Large code model) */
#include "lcd.h"
#include "xlcd.h"
#include <delays.h>
void DelayXLCD (void);
void DelayFor18TCY( void );
void DelayPORXLCD (void);
void clearLCD(void);

void disableLCD(void)
{
    WriteCmdXLCD( DOFF );
}

void enableLCD(void)
{
    clearLCD();
}

void clearLCD(void)
{
    OpenXLCD(FOUR_BIT & LINES_5X7);
    WriteCmdXLCD( FOUR_BIT & LINES_5X7 );
    WriteCmdXLCD( BLINK_OFF );
    WriteCmdXLCD( SHIFT_DISP_LEFT );
    while(BusyXLCD());
    Delay10KTCYx(80);
}

void printLCD(char * str, char * str2){
    clearLCD();
    SetDDRamAddr(0x80);//0x80 is first row.
  //  Delay10KTCYx(80);
    while(BusyXLCD());
    putsXLCD(str);
  //  Delay10KTCYx(80);
    while(BusyXLCD());
    SetDDRamAddr(0xC0);//0xC0 is second row.
  //  Delay10KTCYx(80);
    while(BusyXLCD());
    putsXLCD(str2);
 //   Delay10KTCYx(80);
    while(BusyXLCD());
}


void DelayFor18TCY( void )
{
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
}

void DelayPORXLCD (void)
{
Delay10KTCYx(75); // Delay of 15ms
// Cycles = (TimeDelay * Fosc) / 4
// Cycles = (15ms * 16MHz) / 4
// Cycles = 60,000
return;
}

void DelayXLCD (void)
{
Delay10KTCYx(25); // Delay of 5ms
// Cycles = (TimeDelay * Fosc) / 4
// Cycles = (5ms * 16MHz) / 4
// Cycles = 20,000
return;
}
