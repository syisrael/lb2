/* Compile options:  -ml (Large code model) */

#include <stdio.h>
#include <stddef.h>
//#include <p18f452.h>

#include <p18f25k22.h>

//  turn off the watch dog timer
//#pragma config WDT = OFF

#define TICK_SECOND 1000
#define TICK_MINUTE (TICK_SECOND * 60)
#define TICK_HOUR (TICK_MINUTE * 60)
#define CARBON_DLEAY_MINUTES (TICK_MINUTE * 8)

#define TRIGGER_MEASURECARBON 8

// Declarations
typdef bool enum { false=0, true=1 };

// Init.c methods
void initialize();
void runTasks();

// Handlers
bool takeSemaphore(bool);
void giveSemaphore(bool);

// Tasks
void scheduler();           // handles timing events and triggers timing related tasks
void measureCarbon();       // triggered by scheduler, measures data and stores result in buffer
void measureSalinity();     // ..
void measureFlowRate();     // ..
void measureTemperature();  // ..
void writeSRAM();           // triggered by write request, writes value from some buffer
void readSRAM();            // triggered by read request, stores value to some buffer
void showDisplay();         // triggered by button ISR
void tickTimer();           // pseudo timer function

// Variables
int tick;
bool sMeasureCarbon;

int main(char *args) {
	initialize();
    
	runTasks();
    
    return 0;
}

void initialize() {
    tick = 0;
    sMeasureCarbon = FALSE;
}

void runTasks() {
    for ( ; ; ) {
        scheduler();
        
        if (priority < 1) {
            measureCarbon();
            measureSalinity();
            measureFlowRate();
            measureTemperature();
        }
        
        writeSRAM();
        readSRAM();
        
        if (priority < 2) {
            showDisplay();
        }
        
        tickTimer();
    }
}

void scheduler() {
    // example of trigger
    if (tick % CARBON_DLEAY_MINUTES == 0) {
        giveSemaphore(sMeasureCarbon);
    }
}

void tickTimer() {
    tick++;
}