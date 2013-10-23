#ifndef GLOBAL_H
#define GLOBAL_H
#include <p18f452.h>
#include <usart.h>
#include <ctype.h>
#include <stdio.h>

#define     TERMINAL_CLEAR          "\033[2J"
#define     TERMINAL_RETURN         "\r\n"
#define     TERMINAL_NUMBER_FORMAT  "%6d"
#define     BUFFER_SIZE             40

typedef enum  {false = 0, TRUE = 1 } boolean;

void terminalSendPString(const char*);
void terminalSendString(char*);
void sendTerminalCommandLine();
void updateTerminal();
void sendTerminalCommandList();
void terminalTask();
#endif
