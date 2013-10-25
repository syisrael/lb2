/* 
 * File:   lcd.h
 * Author: Sam
 *
 * Created on October 20, 2013, 10:43 AM
 */

#ifndef LCD_H
#define	LCD_H

void DelayFor18TCY(void);
void DelayPORXLCD(void);
void DelayXLCD(void);

void setupLCD(void);
void openLCD(void);
void closeLCD(void);
void clearLCD(void);
void printLCD(char*, char*);

#endif	LCD_H

