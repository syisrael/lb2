#include <stdio.h>
#include <stddef.h>
#include <p18f452.h>

//#pragma config WDT = OFF

#define     TICK_SECOND             100000
#define     TICK_MINUTE             (TICK_SECOND * 60)
#define     TICK_HOUR               (TICK_MINUTE * 60)
#define     CARBON_DLEAY_MINUTES    (TICK_MINUTE * 8)

#define     SRAM_STORED_VALUES      16

#define     SRAM_SELECT_CARBON      0
#define     SRAM_SELECT_SALINITY    1
#define     SRAM_SELECT_FLOWRATE    2
#define     SRAM_SELECT_TEMP        3

#define     TRIGGER_MEASURECARBON   8

// Declarations
typdef enum { false= 0, true = 1 } bool;
typdef unsigned char byte;

// main.c methods
void initialize();
void runTasks();

// Tasks
void scheduler();           // handles timing events and triggers timing related tasks
void measureCarbon();       // triggered by scheduler, measures data and stores result in buffer
void measureSalinity();     // ..
void measureFlowRate();     // ..
void measureTemperature();  // ..
void showDisplay();         // triggered by button ISR

// Variables
bool sMeasureCarbon, sMeasureSalinity, sMeasureFlowRate, sMeasureTemperature;

unsigned short carbonValue, salinityValue, flowRateValue;
short temperatureValue;

byte SRAMAddressPtrs[4];

int main(char *args) {
	initialize();
    
	runTasks();
    
    return 0;
}

void initialize() {
    // PIC18
    TMR0ON = 1; // Enable Timer0
    
    // Program memory
    sMeasureCarbon = false;
    
    // Carbon, Salinity, Flow Rate, Temperature
    for (int i=0; i < 4; i++)
        SRAMAddressPtrs[i] = (0 * SRAM_STORED_VALUES);
}

void runTasks() {
    for ( ; ; ) {
        scheduler();
        
        measureCarbon();
        measureSalinity();
        measureFlowRate();
        measureTemperature();
        
        showDisplay();
    }
}

void scheduler() {
    // example of trigger
    if (TMR0H % CARBON_DLEAY_MINUTES == 0) {
        sMeasureCarbon = sHardware = true;
    }
}
