
_main:

;Test.c,13 :: 		void main (void)
;Test.c,15 :: 		ADCON1=0b10001000;
	MOVLW       136
	MOVWF       ADCON1+0 
;Test.c,18 :: 		TRISC = 0;
	CLRF        TRISC+0 
;Test.c,19 :: 		PORTC = 0;
	CLRF        PORTC+0 
;Test.c,20 :: 		PORTC = 0b11111111;
	MOVLW       255
	MOVWF       PORTC+0 
;Test.c,21 :: 		while (1){
L_main0:
;Test.c,22 :: 		PORTC = ~PORTC;
	COMF        PORTC+0, 1 
;Test.c,26 :: 		}
	GOTO        L_main0
;Test.c,27 :: 		}
	GOTO        $+0
; end of _main

_ADCRead:

;Test.c,33 :: 		unsigned int ADCRead(unsigned char ch){
;Test.c,34 :: 		if(ch>7) return 0;  //Invalid Channel
	MOVF        FARG_ADCRead_ch+0, 0 
	SUBLW       7
	BTFSC       STATUS+0, 0 
	GOTO        L_ADCRead2
	CLRF        R0 
	CLRF        R1 
	RETURN      0
L_ADCRead2:
;Test.c,35 :: 		ADCON0=0x00;
	CLRF        ADCON0+0 
;Test.c,37 :: 		ADCON0=(ch<<3);   //Select ADC Channel
	MOVF        FARG_ADCRead_ch+0, 0 
	MOVWF       R0 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R0, 1 
	BCF         R0, 0 
	MOVF        R0, 0 
	MOVWF       ADCON0+0 
;Test.c,39 :: 		ADCON0.ADON=1;  //switch on the adc module
	BSF         ADCON0+0, 0 
;Test.c,41 :: 		ADCON0.GO_DONE=1;  //Start conversion
	BSF         ADCON0+0, 2 
;Test.c,43 :: 		while(ADCON0.GO_DONE); //wait for the conversion to finish
L_ADCRead3:
	BTFSS       ADCON0+0, 2 
	GOTO        L_ADCRead4
	GOTO        L_ADCRead3
L_ADCRead4:
;Test.c,45 :: 		ADCON0.ADON=0;  //switch off adc
	BCF         ADCON0+0, 0 
;Test.c,47 :: 		return ADRESH;
	MOVF        ADRESH+0, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
;Test.c,48 :: 		}
	RETURN      0
; end of _ADCRead
