#include <p18f452.h>
#include <delays.h>
#include <timers.h>
#include "motors.h"

#define DIR1 PORTCbits.RC3
#define TDIR1 TRISCbits.RC3

#define DIR2 PORTBbits.RB0
#define TDIR2 TRISBbits.RB0

#define STEP1 PORTCbits.RC2
#define TSTEP1 TRISCbits.RC2

#define STEP2 PORTBbits.RB1
#define TSTEP2 TRISBbits.RB1

#define TQ PORTCbits.RC1
#define TTQ TRISCbits.RC1

void doSteps(MOTOR m, long steps);

int stepDelay = 1000;

void step(MOTOR m, SPEED s, long steps)
{
    stepDelay = s;
    doSteps(m,steps);
}

void setDir(MOTOR m, DIRECTION dir)
{
    if(m == MOTOR1)
        DIR1 = dir;
    else if(m == MOTOR2)
        DIR2 = dir;
}

void doSteps(MOTOR m, long steps)
{
    int i;
    if(m == MOTOR1){
        for(; steps > 0; steps--){
            STEP1 = 1;
            for(i=stepDelay;i>0;i--)
            Delay100TCYx(1);
            STEP1 = 0;
        }
    }else if(m == MOTOR2){
        for(; steps > 0; steps--){
            STEP2 = 1;
            for(i=stepDelay;i>0;i--)
            Delay100TCYx(1);
            STEP2 = 0;
        }
    }
}

void setTorque(TORQUE t)
{
    switch(t){
        case TORQUE_LOW:
            TQ = 1;
            break;
        case TORQUE_HIGH:
            TQ = 0;
            break;
    }
}

void setupMotors()
{
    TDIR1 = 0;
    TDIR2 = 0;
    TSTEP1 = 0;
    TSTEP2 = 0;
    TTQ = 0;
}