//Pic sensor controller
#include <p18f452.h>
#include <delays.h>

#pragma config WDT=OFF              // Watchdog off
#pragma config BOR=OFF              // Brown out reset
//#pragma config LVP=OFF              //
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
#pragma config CP2=OFF              // Code protection
#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection

#define 	enable 		PORTDbits.RD0
#define 	rst			PORTAbits.RA2
#define		clkout		PORTAbits.RA1

#define		clkin		PORTBbits.RB4

#define		test		PORTAbits.RA3			

#define 	a  			PORTCbits.RC0
#define 	b  			PORTCbits.RC1
#define 	c  			PORTCbits.RC2
#define 	d  			PORTCbits.RC3
#define 	e  			PORTCbits.RC4
#define 	f  			PORTCbits.RC5
#define 	g  			PORTCbits.RC6
#define 	h  			PORTCbits.RC7

#define		edge1  		PORTBbits.RB0 //on a side
#define		edge2  		PORTBbits.RB1 //on a side 
#define		edge3  		PORTBbits.RB2 //on h side
#define		edge4  		PORTBbits.RB3 //on h side

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
	TRISC = TRISB = 0xFF;

	TRISA = 0x00;
        TRISDbits.RD0 = 0;

	while(1){
		constructRows();
		Delay10KTCYx(10);
                Delay10KTCYx(10);
	}

}

void constructRows(){
    clkout = 0;
    enable = 1;
    Delay1KTCYx(1);

    row0 = readSensors();
    Delay10KTCYx(1);
    row1 = readSensors();
    Delay10KTCYx(1);
    row2 = readSensors();
    Delay10KTCYx(1);
    row3 = readSensors();
    Delay10KTCYx(1);

    row4 = readSensors();
    Delay10KTCYx(1);
    row5 = readSensors();
    Delay10KTCYx(1);
    row6 = readSensors();
    Delay10KTCYx(1);
    row7 = readSensors();

    Delay1KTCYx(1);
    enable = 0;
    Delay1KTCYx(10);
    rst = 1;
    Delay1KTCYx(10);
    rst = 0;
}

int readSensors(){
    int row = 0;
    clkout = 0;
    Delay10KTCYx(1);
    //low true

//not working
    row = PORTCbits.RC0;
    row = row << 0b1;
    row |= b;
    row = row << 0b1;
    row |= c;
    row  = row << 0b1;
    row |= d;
    row  = row << 0b1;
   // test = rows[i][0];

    row |= e;
    row << 0b1;
    row |= f;
    row << 0b1;
    row |= g;
    row << 0b1;
    row |= h;
    row << 0b1;

    row |= edge1;//on a side
    row << 0b1;
    row |= edge2;//on a side
    row << 0b1;
    row |= edge3;//on h side
    row << 0b1;
    row |= edge4;//on h side
    
    clkout = 1;
    Delay10KTCYx(1);

    return row;
}