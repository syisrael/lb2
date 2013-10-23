/* Compile options:  -ml (Large code model) */
//#include <p18f452.h>
//#include <p18f25k22.h>
#include <p18f452.h>
#include <usart.h>
#include <timers.h>
#include <ctype.h>
#include <stdio.h>
#include <timers.h>

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
unsigned char x = 0;
void delay() {
    Delay10KTCYx(0xff);
    Delay10KTCYx(0xff);
    Delay10KTCYx(0xff);
    Delay10KTCYx(0xff);

#define     TERMINAL_CLEAR          "\033[2J"
#define     TERMINAL_RETURN         "\r\n"
#define     TERMINAL_NUMBER_FORMAT  "%6d"

typedef enum  {false = 0, true = 1 } boolean;

int m[] = { 0xf7ca, 0x1bcc, 0x003a, 0x1010 };
char buffer[40];
boolean flagTemperatureFahrenheit = false;

terminalSendPString(char *str) {
    while(BusyUSART());
    putrsUSART(str);
}

/*void setupUSART() {
    TRISCbits.RC6 = 0;
    TRISCbits.RC7 = 0;
    TXSTAbits.SYNC = 0;
    PIE1bits.TXIE = 1;
    TXSTAbits.TXEN = 1;
    TXSTAbits.BRGH = 1;
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    RCSTAbits.ADDEN = 0;
    SPBRG = 9;
terminalSendString(char *str) {
    while(BusyUSART());
    putsUSART(str);
}

void transmitUSART() {
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
void sendTerminalCommandLine() {
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("(? - list options)");
    terminalSendPString(TERMINAL_RETURN);
    terminalSendPString("Command:\\>");
}
void sendUSART(char data) {
    transmitUSART();
    while(!PIR1bits.TXIF);
    TXREG = data;

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

void freeUSART() {
    while(BusyUSART());
}

void main() {
    char c = 'A';
    TRISCbits.RC7 = 0;
    setupUSART();
    int i = 0;
    char cmd = 0xff;
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &
              USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH &
              USART_ADDEN_OFF, 129);
    updateTerminal();
    while (1) {
        //sendUSART(0b01010101);
        //sendUSART(0b01111110);
        sendUSART(c++);
        //transmitUSART(0x00);
        //
        //delay();
    }
}*/
        if (DataRdyUSART()) {\
            while(BusyUSART());
            cmd = getcUSART();
            while(BusyUSART());
            putcUSART(cmd);
        }

void main() {
    char c = 'a';
    char str[] = "hello world";
    /*OpenUSART(USART_TX_INT_OFF &
              USART_RX_INT_OFF &
              USART_ASYNCH_MODE &
              USART_EIGHT_BIT &
              USART_CONT_RX &
              USART_BRGH_HIGH &
              USART_ADDEN_OFF,
              12);*/
    OpenUSART(USART_TX_INT_OFF &
              USART_RX_INT_OFF &
              USART_ASYNCH_MODE &
              USART_EIGHT_BIT &
              USART_CONT_RX &
              USART_BRGH_HIGH &
              USART_ADDEN_OFF,
              12);
    while (1) {
        while(BusyUSART());
        //putcUSART(0b11111111);
        //delay();
        putcUSART(0xff);
        //delay();
        //if (DataRdyUSART()) {
            //c = getcUSART();
        //
        //putcUSART('U');
        //delay();
        putcUSART('A');
        putcUSART(0x00);
        //delay();
        putcUSART(0b01010101);
        //delay();
        //putsUSART("\033[2J");
        //Delay10TCYx(0xff);
        /*if (i == 0xffff) {
            updateTerminal();
        }*/
        switch (cmd) {
        case '?':
            sendTerminalCommandList();
            break;
        case '1':
            //terminalSendPString(TERMINAL_CLEAR);
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
        
        i++;
    }
}
} /* */