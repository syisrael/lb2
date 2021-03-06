//Pic sensor controller
#include <p18f452.h>
#include <delays.h>
#include "Sensors.h"

//Columns:  0     1   2 3 4 5 6 7 8 9  10    11
//row0->> edge1 edge2 a b c d e f g h edge3 edge4
//row1->> edge1 edge2 a b c d e f g h edge3 edge4
//...
//row7->> edge1 edge2 a b c d e f g h edge3 edge4

//int nrow0,nrow1,nrow2,nrow3,nrow4,nrow5,nrow6,nrow7;
//int orow0,orow1,orow2,orow3,orow4,orow5,orow6,orow7;
//int r0count[12],r1count[12],r2count[12],r3count[12],r4count[12],r5count[12],r6count[12],r7count[12];

int nrow0,nrow1,nrow2,nrow3,nrow4,nrow5,nrow6,nrow7;
int orow0,orow1,orow2,orow3,orow4,orow5,orow6,orow7;
int brow0,brow1,brow2,brow3,brow4,brow5,brow6,brow7;
int r0count[12],r1count[12],r2count[12],r3count[12],r4count[12],r5count[12],r6count[12],r7count[12];

//Sets up sensors inputs/outputs and set initial states
void setupSensors(){
    //Nissa's PIC
   /* TRISC = TRISB = 0xFF; //input
    TRISA = 0x00;         //output
    TRISD = 0x00;*/

    //Set AN pins to Digital I/O
    ADCON1bits.PCFG0 = 1;
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG3 = 0;
      
    //Sam's PIC
    tin0 = 0;
    tin1 = 0;
    tin2 = 0;
    tin3 = 0;

    TRISCbits.RC0 = 1;

    TRISCbits.RC4 = 0;

    TRISEbits.RE0 = 0;
    TRISEbits.RE1 = 0;
    TRISEbits.RE2 = 0;
    

    nrow0 = readSensors();
    nrow1 = readSensors();
    nrow2 = readSensors();
    nrow3 = readSensors();
    nrow4 = readSensors();
    nrow5 = readSensors();
    nrow6 = readSensors();
    nrow7 = readSensors();

    newRead();
    saveState();
    saveBackup();
    clearCounts();
}

//Saves current state to previous state
void saveState(){
    orow0 = nrow0;
    orow1 = nrow1;
    orow2 = nrow2;
    orow3 = nrow3;
    orow4 = nrow4;
    orow5 = nrow5;
    orow6 = nrow6;
    orow7 = nrow7;
}

void saveBackup(){
    brow0 = nrow0;
    brow1 = nrow1;
    brow2 = nrow2;
    brow3 = nrow3;
    brow4 = nrow4;
    brow5 = nrow5;
    brow6 = nrow6;
    brow7 = nrow7;
}

char makeCounts(int* counts, int rowNew, int rowOld, int rowBackup){
    int i = 0;
    int temp = 0;
    if(rowNew != rowOld){
        temp = rowNew ^ rowOld;
        for(i = 0; i < 12; i++){
            if((temp & 1) == 1){
                counts[i]++;
            }
            temp >>= 1;
        }
    } else if (rowNew == rowBackup) {
        return 0;
    }
    return 1;
}

void clearCounts(){
    int i = 0;
    for(i = 0; i <12; i++){
        r0count[i]  = 0;
        r1count[i] = 0;
        r2count[i] = 0;	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              r2count[i] = 0;
        r3count[i] = 0;
        r4count[i] = 0;
        r5count[i] = 0;
        r6count[i] = 0;
        r7count[i] = 0;
    }
}

/*void clearOne(char row, int column){
    if(row == 0) {
        r0count[column] = 0;
    }
    if(row == 1) {
        r1count[column] = 0;
    }
    if(row == 2) {
        r2count[column] = 0;
    }
    if(row == 3) {
        r3count[column] = 0;
    }
    if(row == 4) {
        r4count[column] = 0;
    }
    if(row == 5) {
        r5count[column] = 0;
    }
    if(row == 6) {
        r6count[column] = 0;
    }
    if(row == 7) {
        r7count[column] = 0;
    }
}*/

void enableRead(){
    rst = 1;
    //Delay10TCYx(1);
    Nop();
    rst = 0;
    clkout = 0;
    enable = 1;
    Nop();
    //Delay10TCYx(1);
}

void disableRead(){
    enable = 0;
    Nop();
    //Nop();
    //Delay10KTCYx(1);
}

void newRead(){
    char sum = 0;
    saveState();
    enableRead();

    //Delay10TCYx(1);
    nrow7 = readSensors();
    sum += makeCounts(r7count,nrow7,orow7,brow7);
    //Delay10TCYx(1);
    nrow6 = readSensors();
    sum += makeCounts(r6count,nrow6,orow6,brow6);
    //Delay10TCYx(1);
    nrow5 = readSensors();
    sum += makeCounts(r5count,nrow5,orow5,brow5);
    //Delay10TCYx(1);
    nrow4 = readSensors();
    sum += makeCounts(r4count,nrow4,orow4,brow4);
    //Delay10TCYx(1);

    nrow3 = readSensors();
    sum += makeCounts(r3count,nrow3,orow3,brow3);
    //Delay10TCYx(1);
    nrow2 = readSensors();
    sum += makeCounts(r2count,nrow2,orow2,brow2);
    //Delay10TCYx(1);
    nrow1 = readSensors();
    sum += makeCounts(r1count,nrow1,orow1,brow1);
    //Delay10TCYx(1);
    nrow0 = readSensors();
    disableRead();
    sum += makeCounts(r0count,nrow0,orow0,brow0);
    //Delay10TCYx(1);

    

    if (sum == 0) {
        clearCounts();
    }

    
}

//#define DELAY_READ Delay10TCYx(1)
#define DELAY_READ Nop();Nop();Nop();Nop();Nop()

//series in
int readSensors(){
    int row = 0;
    DELAY_READ;
    clkout = 1;

    in0=1;
    in1=1;
    in2=0;
    in3=1;
    DELAY_READ;
    row = sensor;
    row <<= 1;

    in0=0;
    in1=1;
    in2=0;
    in3=1;
    DELAY_READ;
    row |= sensor;
    row <<= 1;

    in0=1;
    in1=0;
    in2=0;
    in3=1;
    DELAY_READ;
    row |= sensor;
    row <<= 1;

    in0=0;
    in1=0;
    in2=0;
    in3=1;
    DELAY_READ;
    row |= sensor;
    row <<= 1;

    in0=1;
    in1=1;
    in2=1;
    in3=0;
    DELAY_READ;
    row |= sensor;
    row <<= 1;

    in0=0;
    in1=1;
    in2=1;
    in3=0;
    DELAY_READ;
    row |= sensor;
    row <<= 1;

    in0=1;
    in1=0;
    in2=1;
    in3=0;
    DELAY_READ;
    row |= sensor;
    row <<= 1;

    in0=0;
    in1=0;
    in2=1;
    in3=0;
    DELAY_READ;
    row |= sensor;
    row <<= 1;

    in0=1;
    in1=1;
    in2=0;
    in3=0;
    DELAY_READ;
    row |= sensor;
    row <<= 1;

    in0=0;
    in1=1;
    in2=0;
    in3=0;
    DELAY_READ;
    row |= sensor;
    row <<= 1;

    in0=1;
    in1=0;
    in2=0;
    in3=0;
    DELAY_READ;
    row |= sensor;
    row <<= 1;

    in0=0;
    in1=0;
    in2=0;
    in3=0;
    DELAY_READ;
    row |= sensor;

    DELAY_READ;
    clkout = 0;
    DELAY_READ;
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