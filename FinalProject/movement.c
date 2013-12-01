#include <p18f452.h>
#include <delays.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <timers.h>
#include "motors.h"
#include "lcd.h"
#include "movement.h"
#define PI 3.14159265


extern int str[30];

int currentX = 0;
int currentY = 0;

void tester(SPEED);
long abs(long k);
int gcd(long a,long b);

void setupMovement(){
    setupMotors();
    resetPosition();
}

void resetPosition()
{
    setTorque(TORQUE_LOW);
    moveTo(-50000,-50000);
}

void moveTo(long toX, long toY)
{
    long copyX = toX, copyY = toY;
    long div = gcd(copyX,copyY);
    long x1 = toX/div;
    long y1 = toY/div;
    int i;
    SPEED s = FULL_SPEED;
    if(toX > 0){
        setDir(MOTOR1,CW);
    }else{
        setDir(MOTOR1,CCW);
    }
    if(toY > 0){
        setDir(MOTOR2,CW);
    }else{
        setDir(MOTOR2,CCW);
    }
    sprintf(str,"To:%l,%l",toX,toY);
    printLCD(str,str);
    
    for(i = 0; i < div; i++){
        step(MOTOR1,s,x1);
        step(MOTOR2,s,y1);
    }
    /*int x = toX - currentX;
    int y = toY - currentY;
    double len = sqrt(x^2+y^2);
    double angle;
    int x2,y2;

    if(y>0)
        angle = acos(x/len);
    else if(x>0)
        angle = asin(y/len);
    else
        angle = atan(y/x) - PI;

    angle += PI/4;

    if(angle > 0 && angle < PI) //y2 increasing
        setDir(MOTOR1,CW);
    else
        setDir(MOTOR1,CCW);
    if(angle > -PI/2 && angle < PI/2) //x2 increasing
        setDir(MOTOR2,CW);
    else
        setDir(MOTOR2,CCW);

    x2 = abs(cos(angle)) * len;
    y2 = abs(sin(angle)) * len;*/


    /*setTorque(TORQUE_HIGH);
    sprintf(str,"Mode: FULL SPEED   ");
    printLCD(str,str);
    tester(FULL_SPEED);

    
    sprintf(str,"Mode: FAST         ");
    printLCD(str,str);
    tester(FAST);
    sprintf(str,"Mode: STANDARD     ");
    printLCD(str,str);
    tester(STANDARD);
    sprintf(str,"Mode: SLOW         ");
    printLCD(str,str);
    tester(SLOW);
    sprintf(str,"DONE!             ");
    printLCD(str,str);*/
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


int gcd(long a,long b)
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