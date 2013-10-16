/* Compile options:  -ml (Large code model) */
//#include <p18f452.h>
//#include <p18f25k22.h>
#include <p18f452.h>

//  turn off the watch dog timer
#pragma config WDT=OFF              // Watchdog off
#pragma config BOR=OFF              // Brown out reset
#pragma config LVP=OFF               //
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
#pragma config CP2=OFF              // Code protection
#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection
#pragma config PWRT=OFF              // Power up timer off

//#pragma config FOSC = INTOSCIO_EC

//  declare function prototype
unsigned int ADCRead(unsigned char ch);

void main (void)
{
    int i = 0,b = 0;
    //PORTA = 0b0000000;
    TRISA = 0b1111111;
    ADCON0 = 0b00000000;
    ADCON1 = 0b10000000;
    ADCON0bits.ADON = 0x01;

            TRISC = 0;
          PORTC = 0;
        TRISD = 0;
        PORTD = 0;

          while (1){
            ADCON0bits.GO_DONE = 1;
            //ADCON0 = 0b00001001;
            //ADCON0bits.ADON = 1;
            while(ADCON0bits.GO_DONE != 0);
            //ADCON0bits.ADON = 0;
            //ADCON0 = 0b00001000;
            PORTC = ADRESL;
            PORTD = ADRESH;
            i++;
            for(b = 0; b < 10000;b++);
        }
}

//Function to Read given ADC channel AN0=000, AN1=001, AN3=011
unsigned int ADCRead(unsigned char ch){
   //if(ch>7) return 0;  //Invalid Channel
   ADCON0=0x00;

   ADCON0=(ch<<3);   //Select ADC Channel

   ADCON0bits.ADON = 1;  //switch on the adc module

   ADCON0bits.GO_NOT_DONE=1;  //Start conversion

   while(ADCON0bits.GO_NOT_DONE); //wait for the conversion to finish

   ADCON0bits.ADON=0;  //switch off adc

   return ADRESL;
}