#line 1 "C:/Users/Sam/Documents/GitHub/lb2/Lab2/Test.c"










unsigned int ADCRead(unsigned char ch);

void main (void)
{
 ADCON1=0b10001000;


 TRISC = 0;
 PORTC = 0;
 PORTC = 0b11111111;
 while (1){
 PORTC = ~PORTC;



 }
}





unsigned int ADCRead(unsigned char ch){
 if(ch>7) return 0;
 ADCON0=0x00;

 ADCON0=(ch<<3);

 ADCON0.ADON=1;

 ADCON0.GO_DONE=1;

 while(ADCON0.GO_DONE);

 ADCON0.ADON=0;

 return ADRESH;
}
