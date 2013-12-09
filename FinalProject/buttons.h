/* 
 * File:   buttons.h
 * Author: nissav
 *
 * Created on November 23, 2013, 2:10 PM
 */

#ifndef BUTTONS_H
#define	BUTTONS_H

void menuUp(void);
void menuDown(void);
void menuSelect(void);
void buttonSetup(void);
void interruptHandler(void);
void InterruptVectorHigh (void);
void timerISR (void);
void groupSettings(void);
void lineStates(void);
void menu(void);


#endif	/* BUTTONS_H */

