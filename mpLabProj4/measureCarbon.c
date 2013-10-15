// to put in initialzation TEST
/*
MOVLB 0xF ; Set BSR for banked SFRs
CLRF PORTA ; Initialize PORTA by
; clearing output
; data latches
CLRF LATA ; Alternate method
; to clear output
; data latches
MOVLW E0h ; Configure I/O 
MOVWF ANSELA ; for digital inputs
MOVLW 0CFh ; Value used to 
; initialize data 
; direction
MOVWF TRISA ; Set RA<3:0> as inputs
; RA<5:4> as outputs

*/

void measureCarbon()
{
// send power on to measure circuit

//digital I/O to power controller initialzed as output
RB1 = 1;

// wait until circuit is stable
//Delay by amount


//Perform Measurement
// convert to correct format 
//Choose a channel adc channel AN0
ADCON0 = 0;

//Turn on ADC
ADON = 1;

//Start conversion
GODONE = 1;

//Wait for converstion
while(GODONE);

//Turn off ADC
ADON = 0;

//Grab output
carbonMeasure = ADRES;

// power off measureing circuit
RA0 = 0;

//write to SRAM 
writeSRAM() //attach carbonMeasure






}


//Move to initialization section
//Need to figure out how to set the clock to FRC clock source

void ADCinit(){
   /* ADCON2
    * ADC Result Right Justified.
    * Acquisition Time = 2TAD
    * Conversion Clock = 32 Tosc
    */

   ADCON2=0b10001010;
  
  
}


