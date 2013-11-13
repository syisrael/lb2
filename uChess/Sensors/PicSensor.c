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

#define 	enable 		PORTAbits.RA0
#define 	rst			PORTAbits.RA2
#define		clkout		PORTAbits.RA1

#define		clkin		PORTAbits.RB4

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
 
int rows [8][12]; //[rows][columns]

void readSensors(void);

void main(){
	TRISC = TRISB = 0x01;
	TRISA = 0x00;

	
	while(1){
		readSensors();
		Delay10KTCYx(10);
                Delay10KTCYx(10);
	}

}

void readSensors(){
	int i = 0;
    int j = 0;
    clkout = 0;
	enable = 1;
    Delay1KTCYx(1);
    
	for(i = 0; i < 8; i++){
		while(clkin == 1);
		clkout = 1;
		Delay1KTCYx(1);
		//low true
		rows[i][0] = a;
		rows[i][1] = b;
		rows[i][2] = c;
		rows[i][3] = d;
		
		rows[i][4] = e;	
		rows[i][5] = f;
		rows[i][6] = g;
		rows[i][7] = h;
		
		clkout = 0;
		
		rows[i][8] = edge1;//on a side
		rows[i][9] = edge2;//on a side
		rows[i][10] = edge3;//on h side
		rows[i][11] = edge4;//on h side
		Delay10KTCYx(1);
	}
    Delay1KTCYx(1);
    
	enable = 0;
	test = rows[0][0];
	rst = 1;
	Delay1KTCYx(1);
	rst = 0;
}