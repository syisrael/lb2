/* 
 * File:   Sensors.h
 * Author: nissav
 *
 * Created on November 18, 2013, 2:59 PM
 */

#ifndef SENSORS_H
#define	SENSORS_H

//Nissa's PIC Serial
/*
#define 	enable 		PORTDbits.RD0
#define 	rst		PORTDbits.RD2
#define		clkout		PORTDbits.RD1

#define         in0             PORTDbits.RD4
#define         in1             PORTDbits.RD5
#define         in2             PORTDbits.RD6
#define         in3             PORTDbits.RD7

#define         sensor          PORTCbits.RC0
#define		test		PORTDbits.RD4
*/

//Nissa's PIC Parallel
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

#define		test		PORTDbits.RD4
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


int readSensors(void);
void constructRows(void);
void setupSensors(void);
void disableRead(void);
void enableRead(void);

#endif	/* SENSORS_H */

