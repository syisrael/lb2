//Pic sensor controller
#include <p18f452.h>
#include <delays.h>
#include "Sensors.h"

//Columns:  0     1   2 3 4 5 6 7 8 9  10    11
//row0->> edge1 edge2 a b c d e f g h edge3 edge4
//row1->> edge1 edge2 a b c d e f g h edge3 edge4
//...
//row7->> edge1 edge2 a b c d e f g h edge3 edge4

int nrow0,nrow1,nrow2,nrow3,nrow4,nrow5,nrow6,nrow7;
int orow0,orow1,orow2,orow3,orow4,orow5,orow6,orow7;
short r0count,r1count,r2count,r3count,r4count,r5count,r6count,r7count [12];


void setupSensors(){
    //Nissa's PIC
   /*
    TRISC = TRISB = 0xFF; //input
    TRISA = 0x00;         //output
    TRISD = 0x00;
   */

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
}

void makeCounts(char* counts, int rowNew, int rowOld){
    counts[0] = 0;

}


void enableRead(){
    rst = 1;
    Delay10TCYx(1);
    rst = 0;
    clkout = 0;
    enable = 1;
}

void disableRead(){
    enable = 0;
}


void constructRows(){

    rst = 1;
    Delay10TCYx(1);
    rst = 0;
    clkout = 0;
    enable = 1;
    Delay10TCYx(1);

    orow0 = nrow0;
    orow1 = nrow1;
    orow2 = nrow2;
    orow3 = nrow3;

    orow4 = nrow4;
    orow5 = nrow5;
    orow6 = nrow6;
    orow7 = nrow7;

    nrow0 = readSensors();
    makeCounts(&r0count,nrow0,orow0);
    Delay10TCYx(1);
    nrow1 = readSensors();
    makeCounts(&r1count,nrow1,orow1);
    Delay10TCYx(1);
    nrow2 = readSensors();
    makeCounts(&r2count,nrow2,orow2);
    Delay10TCYx(1);
    nrow3 = readSensors();
    makeCounts(&r3count,nrow3,orow3);
    Delay10TCYx(1);

    nrow4 = readSensors();
    makeCounts(&r4count,nrow4,orow4);
    Delay10TCYx(1);
    nrow5 = readSensors();
    makeCounts(&r5count,nrow5,orow5);
    Delay10TCYx(1);
    nrow6 = readSensors();
    makeCounts(&r6count,nrow6,orow6);
    Delay10TCYx(1);
    nrow7 = readSensors();
    makeCounts(&r7count,nrow7,orow7);
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

///Parallel reading
/*
int readSensors(){
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
