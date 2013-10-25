#include <p18f452.h>
#include <delays.h>
#include <adc.h>
#include <timers.h>

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


#define     measureOn           PORTDbits.RD4 //change if necessary digital I/O for power controller initialzed as output
#define 	window		PORTBbits.RB4

#define 	bit0		PORTCbits.RC0
#define 	bit1		PORTCbits.RC1
#define 	bit2		PORTCbits.RC2
#define 	bit3		PORTCbits.RC3

#define 	bit4		PORTCbits.RC4
#define 	bit5		PORTCbits.RC5
#define 	bit6		PORTCbits.RC6
#define 	bit7		PORTCbits.RC7

#define 	bit8		PORTDbits.RD0
#define 	bit9		PORTDbits.RD1
#define 	bit10		PORTDbits.RD2
#define 	bit11		PORTDbits.RD3

#define     rst         PORTDbits.RD6

//typedef enum { false=0, true=1 } boo;

//extern unsigned int SRAMWriteValue;
//extern bool sSRAMWrite;

void Delay10KTCYx( unsigned char unit );

int ADCRead(char channel);
void measureTemperature();
void measureCarbon();
void measureSalinity();
void measureFlowRate();
float getFlowRate();
float getCarbon();
float getSalinity();
float getTemp();

void measureCarbon()
{
	int carbon;
	// send power on to measure circuit
	measureOn = 1;

	//Perform Measurement and convert to correct format 
	//Choose a channel adc channel AN1
	OpenADC(ADC_FOSC_4 & ADC_RIGHT_JUST & ADC_8ANA_0REF , ADC_CH1 & ADC_INT_OFF);
    SetChanADC(ADC_CH1);
    Delay10KTCYx(5);
    ConvertADC();
    while(BusyADC());
    carbon = ReadADC(); //take value read and divide by 204.8 to get voltage read

	// power off measureing circuit
	measureOn = 0;
}

void measureFlowRate()
{
	int flowRate;
	// send power on to measure circuit
	measureOn = 1;
	
	//turn on window
	window = 1;
        
	Delay10KTCYx(3000000);
	//cycles = (time delay * fosc)
	//cycles = (15ms *16Mhz)
	//cycles = 60,000
	
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
    rst = 0;
	measureOn = 0;
}

void measureSalinity()
{
	float salinity;
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
	float temp;
    int adcValue;
	// send power on to measure circuit
	measureOn = 1;

	// perform measurement on AN5 note that AN3 is the +Vref signal and is 7 volts
    OpenADC(ADC_FOSC_4 & ADC_RIGHT_JUST & ADC_8ANA_0REF , ADC_CH5 & ADC_INT_OFF);
    SetChanADC(ADC_CH5);
    Delay10KTCYx(5);
    ConvertADC();
    while(BusyADC());
    adcValue = ReadADC();
    temp =  adcValue;
        
	// power off measureing circuit
	measureOn = 1;
}

float getTemp(char F){
	//conversion
	//1000000000 = 512 =2.5V
	float fTemp = (float) temp;
	
	if (fTemp < 38.4){ // if less than 0.1875 volts
        fTemp = fTemp / 204.8;
        fTemp = fTemp / 7.5 - 0.025;
        fTemp = 14.831*(temp*10) - 1.4169;
	}else {
        fTemp = fTemp / 204.8;
        fTemp = fTemp / 7.5 - 0.025;
        fTemp = 18.95*(fTemp*10) + 0.2146;
	}
	
	if(F){
		fTemp = fTemp*1.8 + 32;
	}
        
	return fTemp;
}

float getCarbon() {
	float fCarbon = (float) carbon;
	return fCarbon*1.387 + 3.06; 
}

float getSalinity() {
	float fSalinity = (float) salinity;
	return fSalinity*0.1;
} 

float getFlowRate() {
	float fFlowRate = (float) flowRate;
	return fFlowRate/4; 
}



void main() {
    short i = 0, b = 0;
    //TRISC = TRISD = 0xFF; // Enable ports for digital input
    //TRISD = 0b00001111;
    //TRISB = 0x00;
    TRISC = TRISD = TRISB = 0x00; // Enable ports for digital output
   // TRISE = 0xFF;

    rst = 1;
    rst = 0;
    
    while(1) {
       // measureTemperature();
        measureFlowRate();
        Delay10KTCYx(2000000);
    }
}