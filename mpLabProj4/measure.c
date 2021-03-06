#define     measureOn	RE1 //change if necessary digital I/O for power controller initialzed as output
#define 	window		RE2 

#define 	bit0		RC0
#define 	bit1		RC1
#define 	bit2		RC2
#define 	bit3		RC3

#define 	bit4		RC4
#define 	bit5		RC5
#define 	bit6		RC6
#define 	bit7		RC7

#define 	bit8		RC8
#define 	bit9		RC9
#define 	bit10		RC10
#define 	bit11		RC11			  			

extern unsigned int SRAMWriteValue;
extern bool sSRAMWrite;
extern unsigned short ADCON0, ADON, GODONE, ADRES;
extern unsigned int INTCON;

void delay(int i, int k)
{
	int temp = k;
	for(;i>0;i--){
		k = temp;
		for(;k>0;k--);
	}
}

void measureCarbon()
{
	int b = 0;
	// send power on to measure circuit
	measureOn = 1;

	// wait until circuit is stable
	delay(10000, 0);

	//Perform Measurement and convert to correct format 
	//Choose a channel adc channel AN1
	channel = 0b00001000;
	SRAMWriteValue = ADCRead(channel);

	// power off measureing circuit
	measureOn = 0;

	//write to SRAM 
	sSRAMWrite = true; 

}

void measureFlowRate()
{
	unsigned int count;
	unsigned int b; 

	// send power on to measure circuit
	measureOn = 1;

	//turn on window
	window = 1

	//wait to perform measurement for ~400ms
	delay(0x9c40,0xc8);

	//turn off window
	window = 0;

	// grab count
	count = bit11;
	count <<= 1;
	count = bit10;
	count <<= 1;
	count = bit9;
	count <<= 1;
	count = bit8;
	count <<= 1;
	count = bit7;
	count <<= 1;
	count = bit6;
	count <<= 1;
	count = bit5;
	count <<= 1;
	count = bit4;
	count <<= 1;
	count = bit3;
	count <<= 1;
	count = bit2;
	count <<= 1;
	count = bit1;
	count <<= 1;
	count = bit0;

	// power off measuring circuit
	measureOn = 0;

	//Grab output
	SRAMWriteValue = count;

	//write to SRAM 
	sSRAMWrite = true;
}

void measureSalinity()
{
	unsigned char high, low;
	unsigned short channel;
	int b = 0;
	// send power on to measure circuit
	measureOn = 1;

	// wait until circuit is stable
	delay(10000, 0);

	//Perform Measurement and convert to correct format  on AN0
	channel = 0b00000000;
	SRAMWriteValue = ADCRead(channel);

	// power off measureing circuit
	measureOn = 0;

	//write to SRAM 
	sSRAMWrite = true;  

}

void measureTemperature()
{
	// send power on to measure circuit
	measureOn = 1;

	// wait until circuit is stable
	delay(10000, 0);

	// perform measurement on AN5
	channel = 0b00101000;
	SRAMWriteValue = ADCRead(channel);

	// power off measureing circuit
	measureOn = 1; 

	//write to SRAM 
	sSRAMWrite = true;  

}

void ADCRead(unsigned char channel)
{
	unsigned short writeValue;
	ADCON0 = channel;
	TRISA = 0b1111111;
	ADCON1 = 0b10000000;
	ADCON0bits.ADON = 0x01;
	ADCON0bits.GO_DONE = 1;
	while(ADCON0bits.GO_DONE != 0);
	writeValue = ADRESH;
	writeValue = (SRAMWriteValue << 8) | ADRESL; 
	return writeValue
}


