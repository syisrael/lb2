/* Compile options:  -ml (Large code model) */
//  turn off the watch dog timer
#pragma config WDT=OFF              // Watchdog off
#pragma config BOR=OFF              // Brown out reset
#pragma config LVP=OFF              //
#pragma config CP0=OFF              // Code protection
#pragma config CP1=OFF              // Code protection
#pragma config CP2=OFF              // Code protection
#pragma config CP3=OFF              // Code protection
#pragma config CPB=OFF              // Boot
#pragma config WRTC=OFF             // Configuration Register Write Protection
#pragma config PWRT=OFF             // Power up timer off

#include <p18f452.h>
#include <usart.h>
#include <ctype.h>
#include <stdio.h>
#include <delays.h>
#include <timers.h>

#define     TERMINAL_CLEAR          "\033[2J"
#define     TERMINAL_RETURN         "\r\n"
#define     TERMINAL_NUMBER_FORMAT  "%6d"
#define     BUFFER_SIZE             40

typedef enum  { false = 0, TRUE = 1 } boolean;

int m[] = { 0xf7ca, 0x1bcc, 0x003a, 0x1010 };
boolean flagTemperatureFahrenheit = false;

extern void setupTerminal(void);
extern void terminalTask(void);

void main() {
    setupTerminal();
    while (1) {
        terminalTask();
    }
}

char buffer[BUFFER_SIZE];
char cmd = 0xff;

void terminalSendPString(const char*);
void terminalSendString(char*);
void sendTerminalCommandLine(void);
void updateTerminal(void);
void sendTerminalCommandList(void);
void setupTerminal(void);
void terminalTask(void);

void terminalSendPString(const char* str) {
    while(BusyUSART());
    putrsUSART(str);
}

void terminalSendString(char* str) {
    while(BusyUSART());
    putsUSART(str);
}

void sendTerminalCommandLine() {
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("(? - list options)");
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("Command:\\>");
}

void updateTerminal() {
    // Clear terminal
    terminalSendPString(TERMINAL_CLEAR);

    terminalSendPString("CheckIt-StoreIt Terminal POS v0.81b");
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("Last Measurements");
    terminalSendPString(TERMINAL_RETURN);

    // Carbon
    terminalSendPString("    Carbon:       ");
    sprintf(buffer, TERMINAL_NUMBER_FORMAT, m[0]++);
    terminalSendString(buffer);
    terminalSendPString(" ppm");
    terminalSendPString(TERMINAL_RETURN);

    // Salinity
    terminalSendPString("    Salinity:     ");
    sprintf(buffer, TERMINAL_NUMBER_FORMAT, m[1]--);
    terminalSendString(buffer);
    terminalSendPString(" ppt");
    terminalSendPString(TERMINAL_RETURN);

    // Flow Rate
    terminalSendPString("    Flow Rate:    ");
    sprintf(buffer, TERMINAL_NUMBER_FORMAT, m[2]++);
    terminalSendString(buffer);
    terminalSendPString(" Lps");
    terminalSendPString(TERMINAL_RETURN);

    // Temperature
    terminalSendPString("    Temperature:  ");
    sprintf(buffer, TERMINAL_NUMBER_FORMAT, m[3]--);
    if (flagTemperatureFahrenheit) {
        terminalSendString(buffer);
        terminalSendPString(" F");
    } else {
        terminalSendString(buffer);
        terminalSendPString(" C");
    }
    terminalSendPString(TERMINAL_RETURN);

    // Clear line
    sendTerminalCommandLine();
}

void sendTerminalCommandList() {
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("1 - Measure Carbon");
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("2 - Measure Salinity");
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("3 - Measure Flow Rate");
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("4 - Measure Temperature");
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("d - Show LCD display");
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("u - Change temperature units (C/F)");
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("c - Clear terminal");
    sendTerminalCommandLine();
}

void setupTerminal() {
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &
                  USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH &
                  USART_ADDEN_OFF, 129);
}

void terminalTask() {
    if (DataRdyUSART()) {\
        while(BusyUSART());
        cmd = getcUSART();
        while(BusyUSART());
        putcUSART(cmd);
    }

    switch (cmd) {
    case '?':
        sendTerminalCommandList();
        break;
    case '1':
        terminalSendPString(TERMINAL_RETURN);
        terminalSendPString("Measuring carbon...");
        terminalSendPString(TERMINAL_RETURN);
        Delay10KTCYx(0x00ff);
        updateTerminal();
        break;
    case 'c':
        updateTerminal();
        break;
    case 'd':
        terminalSendPString(TERMINAL_RETURN);
        terminalSendPString("Revealing display");
        sendTerminalCommandLine();
        break;
    case 'u':
        terminalSendPString(TERMINAL_RETURN);
        flagTemperatureFahrenheit = ~flagTemperatureFahrenheit;
        terminalSendPString("Changed temperature unit");
        sendTerminalCommandLine();
        break;
    case 0xff:
        break;
    default:
        terminalSendPString(TERMINAL_RETURN);
        terminalSendPString("Invalid command");
        sendTerminalCommandLine();
    }
    cmd = 0xff;
}
