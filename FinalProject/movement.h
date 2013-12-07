#ifndef MOVEMENT_H
#define	MOVEMENT_H

typedef enum{UP = 0,DOWN = 4,LEFT = 6,RIGHT = 2,DIAG_RIGHT = 1,DIAG_LEFT = 7,DIAG_RIGHT_DWN = 3,DIAG_LEFT_DWN = 5}MOVEMENT;
//typedef enum{UP,DOWN,LEFT,RIGHT,DIAG_RIGHT,DIAG_LEFT,DIAG_RIGHT_DWN,DIAG_LEFT_DWN}MOVEMENT;

void setupMovement(void);
void resetPosition(void);
void moveTo(long x, long y);
void extendSolenoid(void);
void retractSolenoid(void);
void move(MOVEMENT d);

#endif	/* MOVEMENT_H */

