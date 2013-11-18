//Pic sensor controller
#include <p18f452.h>
#include <delays.h>

#pragma config WDT=OFF              // Watchdog off
#pragma config BOR=OFF              // Brown out reset
#pragma config LVP=OFF              //
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
#pragma config CP2=OFF              // Code protection
#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection

//Nissa's PIC
/*
#define 	enable 		PORTDbits.RD0
#define 	rst		PORTDbits.RD2
#define		clkout		PORTDbits.RD1

#define         in0             PORTDbits.RD4
#define         in1             PORTDbits.RD5
#define         in2             PORTDbits.RD6
#define         in3             PORTDbits.RD7

#define         sensor          PORTCbits.RC0
*/


//Sam's PIC
#define 	enable 		PORTCbits.RC4   //try RE0
#define 	rst		PORTEbits.RE1
#define		clkout		PORTEbits.RE2

#define         in0             PORTBbits.RB2
#define         in1             PORTBbits.RB3
#define         in2             PORTBbits.RB4
#define         in3             PORTBbits.RB5

#define         sensor          PORTCbits.RC0

//#define		test		PORTDbits.RD4


/*
#define		edge1  		PORTCbits.RC0 //on a side   
#define		edge2  		PORTCbits.RC1 //on a side   

#define 	a  		PORTCbits.RC2   
#define 	b  		PORTCbits.RC3   
#define 	c  		PORTCbits.RC4   
#define 	d  		PORTCbits.RC5   
#define 	e  		PORTBbits.RB0   
#define 	f  		PORTBbits.RB1   
#define 	g  		PORTBbits.RB2   
#define 	h  		PORTBbits.RB3   

#define		edge3  		PORTBbits.RB4 //on h side   
#define		edge4  		PORTBbits.RB5 //on h side
*/

//row0->> edge1 edge2 a b c d e f g h edge3 edge4
//row1->> edge1 edge2 a b c d e f g h edge3 edge4
//...
//row7->> edge1 edge2 a b c d e f g h edge3 edge4
 
int row0,row1,row2,row3,row4,row5,row6,row7; //[rows][columns]

//void readSensors(void);
int readSensors(void);
void constructRows(void);

void main(){
    int i,k;
	
        //Nissa's PIC
       /* TRISC = TRISB = 0xFF; //input
	TRISA = 0x00;           //output
        TRISD = 0x00;*/

        //Sam's PIC
        TRISBbits.RB5 = 0;
        TRISBbits.RB4 = 0;
        TRISBbits.RB3 = 0;
        TRISBbits.RB2 = 0;

        TRISCbits.RC0 = 1;

        TRISCbits.RC4 = 0;

        TRISEbits.RE0 = 0;
        TRISEbits.RE1 = 0;
        TRISEbits.RE2 = 0;

	while(1){
		constructRows();
		Delay10KTCYx(10);
	}

}


void constructRows(){
    rst = 1;
    Delay10TCYx(1);
    rst = 0;
    clkout = 0;
    enable = 1;
    Delay10TCYx(1);

    row0 = readSensors();
    Delay10TCYx(1);
    row1 = readSensors();
    Delay10TCYx(1);
    row2 = readSensors();
    Delay10TCYx(1);
    row3 = readSensors();
    Delay10TCYx(1);

    row4 = readSensors();
    Delay10TCYx(1);
    row5 = readSensors();
    Delay10TCYx(1);
    row6 = readSensors();
    Delay10TCYx(1);
    row7 = readSensors();
    Delay10TCYx(1);
    enable = 0;

}

//series in
int readSensors(){
    int row = 0;
    Delay10KTCYx(1);
    clkout = 1;

    in0=0;
    in1=0;
    in2=0;
    in3=0;
    Delay10KTCYx(1);
    row = sensor;
    row <<= 1;

    in0=1;
    in1=0;
    in2=0;
    in3=0;
    Delay10KTCYx(1);
    row |= sensor;
    row <<= 1;

    in0=0;
    in1=1;
    in2=0;
    in3=0;
    Delay10KTCYx(1);
    row |= sensor;
    row <<= 1;

    in0=1;
    in1=1;
    in2=0;
    in3=0;
    Delay10KTCYx(1);
    row |= sensor;
    row <<= 1;

    in0=0;
    in1=0;
    in2=1;
    in3=0;
    Delay10KTCYx(1);
    row |= sensor;
    row <<= 1;

    in0=1;
    in1=0;
    in2=1;
    in3=0;
    Delay10KTCYx(1);
    row |= sensor;
    row <<= 1;

    in0=0;
    in1=1;
    in2=1;
    in3=0;
    Delay10KTCYx(1);
    row |= sensor;
    row <<= 1;

    in0=1;
    in1=1;
    in2=1;
    in3=0;
    Delay10KTCYx(1);
    row |= sensor;
    row <<= 1;

    in0=0;
    in1=0;
    in2=0;
    in3=1;
    Delay10KTCYx(1);
    row |= sensor;
    row <<= 1;

    in0=1;
    in1=0;
    in2=0;
    in3=1;
    Delay10KTCYx(1);
    row |= sensor;
    row <<= 1;

    in0=0;
    in1=1;
    in2=0;
    in3=1;
    Delay10KTCYx(1);
    row |= sensor;
    row <<= 1;

    in0=1;
    in1=1;
    in2=0;
    in3=1;
    Delay10KTCYx(1);
    row |= sensor;

    Delay10KTCYx(1);
    clkout = 0;
    Delay10KTCYx(1);
    return row;
}



/*
int readSensors(){        ///Parallel reading
    int row = 0;
    Delay10KTCYx(1);
    clkout = 1;
    Delay10KTCYx(1);

    //low true

    row = edge1;//on a side
    row <<= 1;
    row |= edge2;//on a side
    row <<= 1;
    
    row |= a;
    row <<= 1;
    row |= b;
    row <<= 1;
    row |= c;
    row <<= 1;
    row |= d;
    row <<= 1;
   // test = rows[i][0];

    row |= e;
    row <<= 1;
    row |= f;
    row <<= 1;
    row |= g;
    row <<= 1;
    row |= h;
    row <<= 1;

    row |= edge3;//on h side
    row <<= 1;
    row |= edge4;//on h side

    Delay10KTCYx(1);

    clkout = 0;

    Delay10KTCYx(1);

    return row;
}*/