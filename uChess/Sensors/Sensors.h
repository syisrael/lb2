#ifndef SENSORS_H
#define	SENSORS_H

//Nissa's PIC Serial

#define 	enable 		PORTDbits.RD0
#define 	rst		PORTDbits.RD2
#define		clkout		PORTDbits.RD1

#define         in0             PORTDbits.RD4
#define         in1             PORTDbits.RD5
#define         in2             PORTDbits.RD6
#define         in3             PORTDbits.RD7

#define         sensor          PORTCbits.RC0
#define		test		PORTDbits.RD4


//Nissa's PIC Parallel
/*
#define 	enable 		PORTDbits.RD0
#define 	rst		PORTDbits.RD2
#define		clkout		PORTDbits.RD1

#define		edge1  		PORTCbits.RC0 //on a side
#define		edge2  		PORTCbits.RC1 //on a side

#define 	a  		PORTCbits.RC2
#define 	b  		PORTCbits.RC3
#define 	c  		PORTCbits.RC4
#define 	d  		PORTCbits.RC5
#define 	e  		PORTBbits.RB0+
#define 	f  		PORTBbits.RB1
#define 	g  		PORTBbits.RB2
#define 	h  		PORTBbits.RB3

#define		edge3  		PORTBbits.RB4 //on h side
#define		edge4  		PORTBbits.RB5 //on h side

#define		test		PORTDbits.RD4
*/

//Sam's PIC
/*
#define 	enable 		PORTCbits.RC4   //try RE0
#define 	rst		PORTEbits.RE1
#define		clkout		PORTEbits.RE2

#define         in0             PORTBbits.RA5
#define         in1             PORTBbits.RB4
#define         in2             PORTBbits.RB3
#define         in3             PORTAbits.RA5

#define         sensor          PORTCbits.RC0
*/

int readSensors(void);
void newRead(void);
void setupSensors(void);
void disableRead(void);
void enableRead(void);
void clearCounts(void);
void clearOne(char row, int column);
void makeCounts(int* counts, int rowNew, int rowOld);
void saveBackup(void);

int nrow0,nrow1,nrow2,nrow3,nrow4,nrow5,nrow6,nrow7;
int orow0,orow1,orow2,orow3,orow4,orow5,orow6,orow7;
int r0count[12],r1count[12],r2count[12],r3count[12],r4count[12],r5count[12],r6count[12],r7count[12];

#endif	/* SENSORS_H */

