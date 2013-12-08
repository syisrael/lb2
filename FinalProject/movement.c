#include <p18f452.h>
#include <delays.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <timers.h>
#include "motors.h"
#include "lcd.h"
#include "movement.h"
#define SOLENOID PORTAbits.RA2
#define T_SOLENOID TRISAbits.RA2
//#define BUTTON_CHECK PORTAbits.RA3
//#define T_BUTTON_CHECK TRISAbits.RA3
#define ENABLE PORTAbits.RA5
#define T_ENABLE TRISAbits.RA5
#define TILE_LENGTH 2545
#define SQRT2 1.41421356237
#define DIAG_LENGTH 10677*1.25/2
#define STARTX 10500
#define STARTY 1740

extern int str[30];

int currentX = 0;
int currentY = 0;

void tester(SPEED);
long abs(long k);
long gcd(long a,long b);

void setupMovement(){
    setupMotors();
    T_SOLENOID = 0;
    T_ENABLE = 0;
    ENABLE = 0;
    retractSolenoid();
    resetPosition();
}

void resetPosition()
{
    int i;
    setTorque(TORQUE_HIGH);
    /*while(currentY > 0){
        move(DOWN);
    }
    while(currentX > 0){
        move(RIGHT);
    }
    move(DOWN);
    move(DOWN);
    move(DOWN);
    move(RIGHT);
    move(RIGHT);
    move(RIGHT);
    move(RIGHT);
    move(RIGHT);
    for(i=0;i<500;i++)
    Delay10KTCYx(10);
    currentY = 0;
    currentX = 0;*/
    moveTo(-STARTX,STARTX);
    moveTo(STARTY,STARTY);
    move(UP);
    move(LEFT);
}

void move(MOVEMENT d)
{
    setTorque(TORQUE_HIGH);
    switch (d){
        case UP:
            moveTo(TILE_LENGTH,TILE_LENGTH);
            currentY++;
            break;
        case DOWN:
            moveTo(-TILE_LENGTH,-TILE_LENGTH);
            currentY--;
            break;
        case RIGHT:
            moveTo(TILE_LENGTH,-TILE_LENGTH);
            currentX--;
            break;
        case LEFT:
            moveTo(-TILE_LENGTH,TILE_LENGTH);
            currentX++;
            break;
        case DIAG_RIGHT:
            moveTo(DIAG_LENGTH,0);
            break;
        case DIAG_LEFT:
            moveTo(0,DIAG_LENGTH);
            break;
        case DIAG_RIGHT_DWN:
            moveTo(0,-DIAG_LENGTH);
            break;
        case DIAG_LEFT_DWN:
            moveTo(-DIAG_LENGTH,0);
            break;
    }
}

void moveTo(long toX, long toY)
{
    long div = gcd(abs(toY),abs(toX));
    long x1 = abs(toX)/div;
    long y1 = abs(toY)/div;
    int i;
    SPEED s = FULL_SPEED;
    ENABLE = 1;
    if(toX > 0){
        setDir(MOTOR1,CCW);
    }else{
        setDir(MOTOR1,CW);
    }
    if(toY > 0){
        setDir(MOTOR2,CCW);
    }else{
        setDir(MOTOR2,CW);
    }
    
    for(i = 0; i < div; i++){
        /*if(BUTTON_CHECK){
            sprintf(str,"%d,%d",i*((int)x1),i*((int)y1));
            printLCD(str,str);
            ENABLE = 0;
            while(BUTTON_CHECK)
                Delay10KTCYx(10);
            ENABLE = 1;
        }*/
        step(MOTOR1,s,x1);
        step(MOTOR2,s,y1);
    }
    ENABLE = 0;
}

void tester(SPEED s)
{
    long k=25600;
    Delay10KTCYx(10);
    setDir(MOTOR1,CW);
    step(MOTOR1,s,k);
    setDir(MOTOR2,CW);
    step(MOTOR2,s,k);
    for(k=0;k<10;k++)
    Delay10KTCYx(10);
    k=25600;
    setDir(MOTOR1,CCW);
    step(MOTOR1,s,k);
    setDir(MOTOR2,CCW);
    step(MOTOR2,s,k);
    for(k=0;k<10;k++)
    Delay10KTCYx(10);
}

void extendSolenoid()
{
    SOLENOID = 1;
}

void retractSolenoid()
{
    SOLENOID = 0;
}


long gcd(long a,long b)
{
    if(b==0)
        return a;
    return gcd(b,a%b);
}
long abs(long k)
{
    if(k >= 0){
        return k;
    }
    k *= -1;
    return k;
}