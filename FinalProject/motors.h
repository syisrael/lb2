#ifndef MOTORS_H
#define	MOTORS_H

typedef enum{FULL_SPEED = 20,FAST = 10,STANDARD = 15,SLOW = 20}SPEED;
typedef enum{TORQUE_LOW,TORQUE_HIGH}TORQUE;
typedef enum{MOTOR1=1, MOTOR2=2}MOTOR;
typedef enum{CW = 1, CCW = 0}DIRECTION;

void setupMotors(void);
void step(MOTOR m, SPEED s, long steps);
void setTorque(TORQUE);
void setDir(MOTOR m, DIRECTION dir);

#endif	/* MOTORS_H */

