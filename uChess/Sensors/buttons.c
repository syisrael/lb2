#include "buttons.h"
#include "lcd.h"
#include <delays.h>
#include <portb.h>
#include <timers.h>
#include <string.h>
#include <p18f452.h>
#include <stdio.h>

char line1[30], line2[30];
char check = 0;
int state = 1;
char sounds = 1;
char hints = 1;
char firstMove = 1;
char settingTime = 1;
char gameMode = 1;

char gameSettings;

//default with all 1's: sounds on, hints on, firstMove player,
//                      settingTime 3 seconds, gameMode Player vs Computer



#pragma interrupt interruptHandler
void interruptHandler(void)
{
    if(INTCON1bits.INT0IF == 1){   //INT 0
        INTCON1bits.INT0IE = 0;
        menuSelect();
        Delay10KTCYx(0);
        INTCON1bits.INT0IF = 0;
        INTCON1bits.INT0IE = 1;
    }

    if(INTCON3bits.INT1IF == 1){   //INT 1
        INTCON3bits.INT1IE = 0;
        menuDown();
        Delay10KTCYx(0);
        INTCON3bits.INT1IF = 0;
        INTCON3bits.INT1IE = 1;
    }

    if(INTCON3bits.INT2IF == 1) {  //INT 2
        INTCON3bits.INT2IE = 0;
        menuUp();
        Delay10KTCYx(0);
        INTCON3bits.INT2IF = 0;
        INTCON3bits.INT2IE = 1;
    }
}
#pragma code


#pragma code HIGH_INTERRUPT_VECTOR = 0x08
void high_ISR(void)
{
    _asm goto interruptHandler _endasm //jump to interrupt routine
}
#pragma code

void groupSettings()
{
    gameSettings = gameMode;
    gameSettings <<= 1;
    gameSettings |= firstMove;
    gameSettings << 1;
    gameSettings |= hints;
    gameSettings << 1;
    gameSettings |= sounds;
    gameSettings << 1;
    gameSettings |= settingTime;
}

void buttonSetup()
{
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    TRISBbits.RB2 = 1;

    groupSettings();

    RCONbits.IPEN = 1;
    INTCON1bits.GIE_GIEH = 1;

    INTCON2bits.RBPU = 1;
    INTCON3bits.INT1IP = 1;
    INTCON3bits.INT1IP = 1;

    INTCON2bits.INTEDG0 = 1;
    INTCON2bits.INTEDG1 = 1;
    INTCON2bits.INTEDG2 = 1;

    INTCON1bits.INT0IE = 1;
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT2IE = 1;

    INTCON1bits.INT0IF = 0;
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT2IF = 0;
    lineStates();
    printLCD(line1,line2);
    
}

void lineStates()
{
    if(state == 1){
        sprintf(line1,"uChess v 0.9b-5");
        sprintf(line2,"Press Select");
    }else if(state == 2){
        sprintf(line1,">Start Game");
        sprintf(line2,"Change Settings");
    }else if(state == 3){
        sprintf(line1,">Change Settings");
        sprintf(line2,"-");
    }else if(state == 4){
        sprintf(line1,">Player vs Comp");
        sprintf(line2,"Comp vs Comp");
    }else if(state == 5){
        sprintf(line1,">Comp vs Comp");
        sprintf(line2,"Back");
    }else if(state == 6){
        sprintf(line1,">Back");
        sprintf(line2,"-");
    }else if(state == 7){
        sprintf(line1,">First Move?");
        sprintf(line2,"Hints?");
    }else if(state == 8){
        sprintf(line1,">Hints?");
        sprintf(line2,"Sounds?");
    }else if(state == 9){
        sprintf(line1,">Sounds?");
        sprintf(line2,"Settle Time?");
    }else if(state == 10){
        sprintf(line1,">Setting Time?");
        sprintf(line2,"Back");
    }else if(state == 11){
        sprintf(line1,">Back");
        sprintf(line2,"-");
    }else if(state == 12){
        sprintf(line1,">Player First");
        sprintf(line2,"Computer First");
    }else if(state == 13){
        sprintf(line1,">Computer First");
        sprintf(line2,"-");
    }else if(state == 14 || state == 16){
        sprintf(line1,">ON");
        sprintf(line2,"OFF");
    }else if(state == 15 || state ==17){
        sprintf(line1,">OFF");
        sprintf(line2,"-");
    }else if(state == 18){
        sprintf(line1,">1 second");
        sprintf(line2,"3 seconds");
    }else if(state == 19){
        sprintf(line1,">3 seconds");
        sprintf(line2,"-");
    }else if(state == 20){
        sprintf(line1,">Player vs Comp");
        sprintf(line2,"Good luck!");
    }else if(state == 21){
        sprintf(line1,">Comp vs Comp");
        sprintf(line2,"Good luck!");
    }else if(state == 22){
        sprintf(line1,">Selected Player");
        sprintf(line2,"Press Select");
    }else if(state == 23){
        sprintf(line1,">Selected Comp");
        sprintf(line2,"Press Select");
    }else if(state == 24){
        sprintf(line1,">Selected On");
        sprintf(line2,"Press Select");
    }else if(state == 25){
        sprintf(line1,">Selected Off");
        sprintf(line2,"Press Select");
    }else if(state == 26){
        sprintf(line1,">Selected 1 sec");
        sprintf(line2,"Press Select");
    }else if(state == 27){
        sprintf(line1,">Selected 3 sec");
        sprintf(line2,"Press Select");
    }
}

void menuUp() //RB2
{
    if(state == 1){
        state++;
    }else if(state !=2 && state != 4 && state !=7 && state !=12 && state != 14 && state != 16 && state != 18 && state && state !=20 && state !=21 && state !=22 && state != 23 && state !=24 && state !=25 && state !=26 && state !=27){
        state--;
    }
    lineStates();
    printLCD(line1,line2);
}

void menuDown() //RB1
{
    if(state == 1){
        state++;
    }else if(state != 3 && state != 6 && state != 11 && state != 13 && state != 15 && state != 17 && state != 19 && state !=20 && state !=21 && state !=22 && state != 23 && state !=24 && state !=25 && state !=26 && state !=27 ){
        state++;
    }
    lineStates();
    printLCD(line1,line2);
}

void menuSelect() //RB0
{
    if(state == 1){
        state++;
    }else if(state == 2){
        state = 4;
    }else if(state == 3){
        state = 7;
    }else if(state == 4){
        //player vs computer
        gameMode = 1;
        state = 20;
        groupSettings();
    }else if(state == 5){
        //Computer vs computer
        gameMode = 0;
        state = 21;
        groupSettings();
    }else if(state == 6){
        state = 2;
    }else if(state == 7){
        state = 12;
    }else if(state == 8){
        state = 14;
    }else if(state == 9){
        state = 16;
    }else if(state == 10){
        state = 18;
    }else if(state == 11){
        state = 2;
    }else if(state == 12){
        //first move player
        firstMove = 1;
        state = 22;
    }else if(state == 13){
        //first move computer
        firstMove = 0;
        state = 23;
    }else if(state == 14){
        //hints on
        hints = 1;
        state = 24;
    }else if(state == 15){
        //hints off
        hints = 0;
        state = 25;
    }else if(state == 16){
        //sounds on
        sounds = 1;
        state = 24;
    }else if(state == 17){
        //sounds off
        sounds = 0;
        state = 25;
    }else if(state == 18){
        //1 second settling time
        settingTime = 0;
        state = 26;
    }else if(state == 19){
        //3 second settling time
        settingTime = 1;
        state = 27;
    }else if(state == 20 || state == 21){
        //starting game return to menu
        state = 2;
    }else if(state = 23 || state == 24 || state == 25 || state ==26 || state == 27){
        //setting option selected return to settings menu
        state = 7;
    }
    lineStates();
    printLCD(line1,line2);
}
/*
void main(){
//setup for testing only
   // TRISDbits.RD4 = 0;
   // TRISDbits.RD5 = 0;
   // TRISDbits.RD6 = 0;
    
  //  PORTDbits.RD4 = 0;
  //  PORTDbits.RD5 = 0;
  //  PORTDbits.RD6 = 0;

    TRISAbits.RA0 = 0;
    TRISAbits.RA1 = 0;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;

    PORTAbits.RA0 = 0;
    PORTAbits.RA1 = 0;
    PORTAbits.RA2 = 0;
    PORTAbits.RA3 = 0;

    TRISDbits.RD3 = 0;
    TRISDbits.RD2 = 0;
    TRISDbits.RD1 = 0;

    PORTDbits.RD3 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD1 = 0;

    ADCON1bits.PCFG0 = 1;   //AN channels to D I/O
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG3 = 0;
 
    buttonSetup();
    setupLCD();

}*/