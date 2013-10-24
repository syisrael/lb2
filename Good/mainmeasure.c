/* Compile options:  -ml (Large code model) */
//#include <p18f452.h>
//#include <p18f25k22.h>
#include <p18f452.h>
#include <delays.h>
#include <adc.h>

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
#define 	window		PORTDbits.RD5

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

void measureCarbon()
{
	unsigned short channel;
	unsigned int SRAMWriteValue;
	// send power on to measure circuit
	measureOn = 1;

	// wait until circuit is stable
	//Delay10KTCYx(25);

	//Perform Measurement and convert to correct format 
	//Choose a channel adc channel AN1
	channel = 0b00001000;
	SRAMWriteValue = ADCRead(channel);

	// power off measureing circuit
	measureOn = 0;
}

void measureFlowRate()
{
	unsigned int count;
        unsigned int b;
	unsigned int SRAMWriteValue;
	// send power on to measure circuit
	measureOn = 1;
        count =
	//turn on window
	window = 1;

	//wait to perform measurement for ~400ms
      //  for(b = 0; b < 0xAFFF; b++){
        //    delay(0xFFFF, 0xFFFF);

        //}
        
	//Delay10KTCYx(2000000);
	//cycles = (time delay * fosc)
	//cycles = (15ms *16Mhz)
	//cycles = 60,000
	
	//turn off window
	window = 0;

	// grab count
	count = bit11;
	count <<= 1;
	count |= bit10;
	count <<= 1;
	count |= bit9;
	count <<= 1;
	count |= bit8;
	count <<= 1;
	count |= bit7;
	count <<= 1;
	count |= bit6;
	count <<= 1;
	count |= bit5;
	count <<= 1;
	count |= bit4;
	count <<= 1;
	count |= bit3;
	count <<= 1;
	count |= bit2;
	count <<= 1;
	count |= bit1;
	count <<= 1;
	count |= bit0;

	// power off measuring circuit
    rst = 1;
    rst = 0;
	measureOn = 0;

	//Grab output
	SRAMWriteValue = count/4;
}

void measureSalinity()
{
	unsigned short channel;
	unsigned int SRAMWriteValue;
	// send power on to measure circuit
	measureOn = 1;

	// wait until circuit is stable
	//Delay10KTCYx(25);

	//Perform Measurement and convert to correct format  on AN0
	channel = 0b00000000;
	SRAMWriteValue = ADCRead(channel);

	// power off measureing circuit
	measureOn = 0;
}

void measureTemperature()
{
	char channel;
	float SRAMWriteValue;
	float temp;
        int adcValue;
	// send power on to measure circuit
	measureOn = 1;

	// wait until circuit is stable
	//Delay10KTCYx(25);

	// perform measurement on AN5
	//channel = 0b00101000;
        channel = 0b00001000;
        OpenADC(ADC_FOSC_4 & ADC_RIGHT_JUST & ADC_8ANA_0REF , ADC_CH1 & ADC_INT_OFF);
        SetChanADC(ADC_CH1);
        Delay10KTCYx(5);
        ConvertADC();
        while(BusyADC());
        adcValue = ReadADC();

	//adcValue = ADCRead(channel);
        temp = (float) adcValue;
        //temp = 519.0;
	//conversion
	//1000000000 = 512 =2.5V
	if (temp < 512){ // if less than 2.5 volts
            temp = temp / 204.8;
            temp = temp - 2.5;
                temp = 14.831*temp - 1.4169;
                PORTDbits.RD0 = 1;
	}
	else {
             temp = temp / 204.8;
             temp = temp - 2.5;
		temp = 18.95*temp + 0.2146;
                PORTDbits.RD1 = 1;
	}
	
	SRAMWriteValue = temp;
        
	// power off measureing circuit
	measureOn = 1;
        PORTDbits.RD0 = 0;
        PORTDbits.RD1 = 0;
}

int ADCRead(char channel)
{
	int writeValue, SRAMWriteValue;
	ADCON0 = channel;
	TRISA = 0b1111111;
        //TRISE = 0b1111111;
	ADCON1 = 0b10000000;
	ADCON0bits.ADON = 0x01;
	ADCON0bits.GO_DONE = 1;
	while(ADCON0bits.GO_DONE != 0);
	writeValue = ADRESH;
	writeValue = (writeValue << 8) | ADRESL;
	return writeValue;

}

void main() {
    short i = 0, b = 0;
    //TRISC = TRISD = 0xFF; // Enable ports for digital input
    //TRISD = 0b00001111;
    //TRISB = 0x00;
    TRISC = TRISD = 0x00; // Enable ports for digital output
   // TRISE = 0xFF;

    rst = 1;
    rst = 0;
    
    while(1) {
        measureTemperature();
		//Delay10KTCYx(100);
    }
}