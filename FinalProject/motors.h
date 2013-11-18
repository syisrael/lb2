#ifndef MOTORS_H
#define	MOTORS_H

typedef enum{FULL_SPEED = 3,FAST = 5,STANDARD = 6,SLOW = 8}SPEED;
typedef enum{TORQUE_LOW,TORQUE_HIGH}TORQUE;
typedef enum{MOTOR1, MOTOR2}MOTOR;
typedef enum{CW = 1, CCW = 0}DIRECTION;

void setupMotors(void);
void step(MOTOR m, SPEED s, int steps);
void setTorque(TORQUE);
void setDir(MOTOR m, DIRECTION dir);

#endif	/* MOTORS_H */

