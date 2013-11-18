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
int abs(int k);

void setupMovement(){
    setupMotors();
    resetPosition();
}

void resetPosition()
{
    setTorque(TORQUE_LOW);
    step(MOTOR1,FULL_SPEED,5000);
}

void moveTo(int toX, int toY)
{
    int x = toX - currentX;
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
    y2 = abs(sin(angle)) * len;


    setTorque(TORQUE_HIGH);
    sprintf(str,"Mode: HIGH TORQUE   ");
    printLCD(str,str);
    tester(STANDARD);
    
    
    setTorque(TORQUE_LOW);
    sprintf(str,"Mode: LOW TORQUE   ");
    printLCD(str,str);
    tester(STANDARD);


    /*
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
    int k=3600;
    Delay10KTCYx(10);
    setDir(MOTOR1,CW);
    step(MOTOR1,s,k);
    for(k=0;k<10;k++)
    Delay10KTCYx(10);
    k=3600;
    setDir(MOTOR1,CCW);
    step(MOTOR1,s,k);
    for(k=0;k<10;k++)
    Delay10KTCYx(10);
}

int abs(int k)
{
    if(k > 0){
        return k;
    }
    k *= -1;
    return k;
}