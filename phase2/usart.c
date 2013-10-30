/* Compile options:  -ml (Large code model) */
//#include <p18f452.h>
//#include <p18f25k22.h>
#include <p18f452.h>
#include <usart.h>
#include <timers.h>
#include <ctype.h>
#include <stdio.h>
#include <timers.h>
#include "communications.h"
#include "measure.h"

#define     TERMINAL_CLEAR          "\033[2J"
#define     TERMINAL_RETURN         "\r\n"
#define     TERMINAL_NUMBER_FORMAT  "%6d"

typedef enum  { false = 0, true = 1 } boolean;

char buffer[40];
char cmd;
extern char fahrenheit;

void terminalSendPString(char *str) {
    while(BusyUSART());
    putrsUSART(str);
}

void terminalSendString(char *str) {
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

    terminalSendPString("CheckIt-StoreIt Terminal v0.81b");
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("Last Measurements");
    terminalSendPString(TERMINAL_RETURN);

    // Carbon
    terminalSendPString("    Carbon:       ");
    sprintf(buffer, TERMINAL_NUMBER_FORMAT, getCarbon());
    terminalSendString(buffer);
    terminalSendPString(" ppm");
    terminalSendPString(TERMINAL_RETURN);

    // Salinity
    terminalSendPString("    Salinity:     ");
    sprintf(buffer, TERMINAL_NUMBER_FORMAT, getSalinity());
    terminalSendString(buffer);
    terminalSendPString(" ppt");
    terminalSendPString(TERMINAL_RETURN);

    // Flow Rate
    terminalSendPString("    Flow Rate:    ");
    sprintf(buffer, TERMINAL_NUMBER_FORMAT, getFlowRate());
    terminalSendString(buffer);
    terminalSendPString(" Lps");
    terminalSendPString(TERMINAL_RETURN);

    // Temperature
    terminalSendPString("    Temperature:  ");
    sprintf(buffer, TERMINAL_NUMBER_FORMAT, getTemp(fahrenheit));
    if (fahrenheit) {
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
    terminalSendPString("c - Clear terminal");
    sendTerminalCommandLine();
}

void freeUSART() {
    while(BusyUSART());
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
            measureCarbon();
            terminalSendPString(TERMINAL_RETURN);
            updateTerminal();
            break;
        case '2':
            terminalSendPString(TERMINAL_RETURN);
            terminalSendPString("Measuring salinity...");
            measureSalinity();
            terminalSendPString(TERMINAL_RETURN);
            updateTerminal();
            break;
        case '3':
            terminalSendPString(TERMINAL_RETURN);
            terminalSendPString("Measuring flow rate...");
            measureFlowRate();
            terminalSendPString(TERMINAL_RETURN);
            updateTerminal();
            break;
        case '4':
            terminalSendPString(TERMINAL_RETURN);
            terminalSendPString("Measuring temperature...");
            measureTemperature();
            terminalSendPString(TERMINAL_RETURN);
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
        case 0xff:
            break;
        default:
            terminalSendPString(TERMINAL_RETURN);
            terminalSendPString("Invalid command");
            sendTerminalCommandLine();
        }
        cmd = 0xff;
}

void setupTerminal() {
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &
              USART_EIGHT_BIT & USART_CONT_RX, 129);
}