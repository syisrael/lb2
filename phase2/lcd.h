/* 
 * File:   lcd.h
 * Author: Sam
 *
 * Created on October 20, 2013, 10:43 AM
 */

#ifndef LCD_H
#define	LCD_H

extern void DelayFor18TCY(void);
extern void DelayPORXLCD(void);
extern void DelayXLCD(void);

extern void setupLCD(void);
extern void openLCD(void);
extern void closeLCD(void);
extern void clearLCD(void);
extern void printLCD(char*, char*);

#endif	LCD_H

