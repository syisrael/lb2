/* Compile options:  -ml (Large code model) */

//#include <p18f452.h>
//#include <p18f25k22.h>
//#include "p16f877a.h"

//  turn off the watch dog timer
//#pragma config WDT = OFF

//  declare function prototype
unsigned int ADCRead(unsigned char ch);

void main (void)
{
        ADCON1=0b10001000;
        //TRISB = 0;
          //PORTB = 0;
          TRISC = 0;
          PORTC = 0;
          PORTC = 0b11111111;
          while (1){
                PORTC = ~PORTC;

            //PORTC = ADCRead(0b011);
            //PORTC = ADCRead(0b011) >> 8;
        }
}




//Function to Read given ADC channel AN0=000, AN1=001, AN3=011
unsigned int ADCRead(unsigned char ch){
   if(ch>7) return 0;  //Invalid Channel
   ADCON0=0x00;

   ADCON0=(ch<<3);   //Select ADC Channel

   ADCON0.ADON=1;  //switch on the adc module

   ADCON0.GO_DONE=1;  //Start conversion

   while(ADCON0.GO_DONE); //wait for the conversion to finish

   ADCON0.ADON=0;  //switch off adc

   return ADRESH;
}