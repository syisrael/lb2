/* 
 * File:   communications.h
 * Author: Sam
 *
 * Created on October 25, 2013, 1:07 AM
 */

#ifndef COMMUNICATIONS_H
#define	COMMUNICATIONS_H


void terminalTask(void);
void freeUSART(void);
void sendTerminalCommandList(void);
void updateTerminal(void);
void sendTerminalCommandLine(void);
void terminalSendString(char *str);
void terminalSendPString(char *str);
void setupTerminal(void);

#endif	/* COMMUNICATIONS_H */

