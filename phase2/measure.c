
#include <p18f452.h>
#include <delays.h>
#include <adc.h>
#include <timers.h>
#include "measure.h"
#include <math.h>

#define     measureOn           PORTAbits.RA4 //change if necessary digital I/O for power controller initialzed as output
#define 	window		PORTAbits.RA2

#define 	bit0		PORTAbits.RA5
#define 	bit1		PORTEbits.RE1
#define 	bit2		PORTEbits.RE2
#define 	bit3		PORTAbits.RA6

#define 	bit4		PORTCbits.RC0
#define 	bit5		PORTCbits.RC1
#define 	bit6		PORTCbits.RC2

#define 	bit7		PORTBbits.RB0
#define 	bit8		PORTBbits.RB5
#define 	bit9		PORTBbits.RB4
#define 	bit10		PORTBbits.RB3
#define 	bit11		PORTBbits.RB2

#define         rst             PORTAbits.RA3


int flowRate, carbon, salinity;
int temp [5];


void measureCarbon()
{

	//Perform Measurement and convert to correct format
	//Choose a channel adc channel AN1
	OpenADC(ADC_FOSC_4 & ADC_RIGHT_JUST & ADC_8ANA_0REF , ADC_CH1 & ADC_INT_OFF);
    SetChanADC(ADC_CH1);
    Delay10KTCYx(5);
    ConvertADC();
    while(BusyADC());
    carbon = ReadADC(); //take value read and divide by 204.8 to get voltage read
}

void measureFlowRate()
{
    int k;

	//turn on window
	window = 1;
        for(k=0;k<5;k++)
            Delay10KTCYx(10);

	//turn off window
	window = 0;

	// grab flowRate
	flowRate = bit11;
	flowRate <<= 1;
	flowRate |= bit10;
	flowRate <<= 1;
	flowRate |= bit9;
	flowRate <<= 1;
	flowRate |= bit8;
	flowRate <<= 1;
	flowRate |= bit7;
	flowRate <<= 1;
	flowRate |= bit6;
	flowRate <<= 1;
	flowRate |= bit5;
	flowRate <<= 1;
	flowRate |= bit4;
	flowRate <<= 1;
	flowRate |= bit3;
	flowRate <<= 1;
	flowRate |= bit2;
	flowRate <<= 1;
	flowRate |= bit1;
	flowRate <<= 1;
	flowRate |= bit0;

	// power off measuring circuit
    rst = 1;
    Delay1KTCYx(1);
    rst = 0;
}

void measureSalinity()
{
	// send power on to measure circuit
	measureOn = 1;

	//Perform Measurement and convert to correct format  on AN0
    OpenADC(ADC_FOSC_4 & ADC_RIGHT_JUST & ADC_8ANA_0REF , ADC_CH0 & ADC_INT_OFF);
    SetChanADC(ADC_CH0);
    Delay10KTCYx(5);
    ConvertADC();
    while(BusyADC());
    salinity = ReadADC(); //take value read and divide by 204.8 to get voltage read

	// power off measureing circuit
	measureOn = 0;
}

void measureTemperature()
{
    int i;
    for(i=0;i<5;i++){
            // perform measurement on AN5 note that AN3 is the +Vref signal and is 7 volts
        OpenADC(ADC_FOSC_4 & ADC_RIGHT_JUST & ADC_8ANA_0REF , ADC_CH5 & ADC_INT_OFF);
        SetChanADC(ADC_CH5);
        Delay10KTCYx(5);
        ConvertADC();
        while(BusyADC());
        temp[i] = ReadADC();
    }
}

int getTemp(char F){
	//conversion
	//1000000000 = 512 =2.5V
	float fTemp = (float) ((((temp[0]*.2+temp[1]*.8)*.2 + temp[3]*.8)*.2 + temp[4]*.8)*.2 + temp[5]*.8);

	if (fTemp < 38.4){ // if less than 0.1875 volts
        fTemp = fTemp / 204.8;
        fTemp = fTemp / 7.5 - 0.025;
        fTemp = 14.831*(fTemp*10) - 1.4169;
	}else {
        fTemp = fTemp / 204.8;
        fTemp = fTemp / 7.5 - 0.025;
        fTemp = 18.95*(fTemp*10) + 0.2146;
	}

	if(F){
		fTemp = fTemp*1.8 + 32;
	}
        fTemp = (int) fTemp;
	return (int)fTemp;
}

int getCarbon() {
	float fCarbon = (float) carbon;
	return (int)(fCarbon*0.54209 + 3.061);
}

int getSalinity() {
	float fSalinity = (float) salinity;
	return (int)(fSalinity*0.08789-17.5);
}

int getFlowRate() {
	return flowRate;
}

void setupMeasurement() {
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 1;
    TRISAbits.TRISA6 = 1;

    TRISEbits.TRISE1 = 1;
    TRISEbits.TRISE2 = 1;

    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 1;

    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB5 = 1;
}