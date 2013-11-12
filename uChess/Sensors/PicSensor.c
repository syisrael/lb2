//Pic sensor controller


#define 	enable 		PORTAbits.RA0
#define		clkout		PORTAbits.RA1
#define 	rst			PORTAbits.RA2

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
 
int [8][12] rows; //[rows][columns]

void readSensors();

void main{
	TRISC = TRISB = 0x01;
	TRISA = 0x00;
	
	while(1){
		readSensors();
		Delay10KTCYx(10);
	}

}

void readSensors(){
	int i = 0;
	enable = 1;
	
	for(i = 0; i < 8; i++){
		//low true
		rows[i][0] = a;
		rows[i][1] = b;
		rows[i][2] = c;
		rows[i][3] = d;
		
		rows[i][4] = e;	
		rows[i][5] = f;
		rows[i][6] = g;
		rows[i][7] = h;
		
		rows[i][8] = edge1;//on a side
		rows[i][9] = edge2;//on a side
		rows[i][10] = edge3;//on h side
		rows[i][11] = edge4;//on h side
	}
	enable = 0;
	test = rows[0][0];
	rst = 1;
	Delay1KTCYx(10);
	rst = 0;
}