/* Compile options:  -ml (Large code model) */

#include <stdio.h>
#include <stddef.h>
//#include <p18f452.h>
//#include <p18f25k22.h>
#include "p16f877a.h"

//  turn off the watch dog timer
//#pragma config WDT = OFF

//  test struct with pointer to function
struct Data
{
	int x;
	char y;
	void (*f2)(int anInt);
};

//  declare function prototype
void f1(int x);
unsigned int ADCRead(unsigned char ch);

void main (void)
{

	//  declare instance of struct
  	struct Data myData;
	int i = 0;
	int j = 0;
	int k = 0;

	int a = 39;
        int b = 3;
        int c = 0;

	char myBuff[20];
	char myBuf1[] = "hello world";

        ADCON1=0b10001000;
	TRISB = 0;
  	PORTB = 0;

	//  assign values to struct members
  	myData.f2 = f1;
  	myData.x = 3;
  	myData.y = 'a';


	//  test call of function in struct
  	myData.f2(myData.x);

	for (k = 0; k < 100; k++)
	{
	  	PORTB = 1;

		for (i = 0; i < 1000; i++)
		{
			j = j + 1;
		}

		PORTB = 2;

		for (i = 0; i < 1000; i++)
		{
			j = j + 1;
		}

	}


  	while (1){
            PORTB = ADCRead(0b011);
        }
}

//  test function
void f1(int y)
{
	y = y+1;
    return;
}


//Function to Read given ADC channel AN0=000, AN1=001, AN3=011
unsigned int ADCRead(unsigned char ch){
   if(ch>7) return 0;  //Invalid Channel
   ADCON0=0x00;

   ADCON0=(ch<<3);   //Select ADC Channel

   ADCON0.ADON=1;  //switch on the adc module

   ADCON0.GODONE=1;  //Start conversion

   while(ADCON0.GODONE); //wait for the conversion to finish

   ADCON0.ADON=0;  //switch off adc

   return (ADRESH << 5) & ADRESL;
}