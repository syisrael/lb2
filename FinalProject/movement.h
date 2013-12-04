#ifndef MOVEMENT_H
#define	MOVEMENT_H

typedef enum{UP,DOWN,LEFT,RIGHT,DIAG_RIGHT,DIAG_LEFT,DIAG_RIGHT_DWN,DIAG_LEFT_DWN}MOVEMENT;

void setupMovement(void);
void resetPosition(void);
void moveTo(long x, long y);
void extendSolenoid(void);
void retractSolenoid(void);
void move(MOVEMENT d);

#endif	/* MOVEMENT_H */

