/* Compile options:  -ml (Large code model) */

#include <stdio.h>
#include <stddef.h>
//#include <p18f452.h>
//#include <p18f25k22.h>
#include <p16f877a.h>

//  turn off the watch dog timer
//#pragma config WDT = OFF

//  test struct with pointer to function
struct Data
{
	int x;
	char y;
	void (*f2)(int anInt);
};

//  declare function prototype
void f1(int x);

void main (void)
{
 
	//  declare instance of struct
  	struct Data myData;
	int i = 0;
	int j = 0;
	int k = 0;
	
	int a = 39;
    int b = 3;
    int c = 0;

	char myBuff[20];
	char myBuf1[] = "hello world";

	sprintf(myBuff,"%#010x", i);
 	
	TRISB = 0;
  	PORTB = 0;

	//  assign values to struct members
  	myData.f2 = f1;
  	myData.x = 3;
  	myData.y = 'a';

	//  test print
  	printf ("Hello, world!\n");
	printf ("%s\n", myBuf1);

	//  test sprintf

	sprintf(myBuff, "hello world");

	printf("say what %s\n", myBuff);

	//  test call of function in struct
  	myData.f2(myData.x);
	
	for (k = 0; k < 100; k++)
	{
	  	PORTB = 1;

		for (i = 0; i < 1000; i++)
		{
			j = j + 1;
		}
		
		PORTB = 2;

		for (i = 0; i < 1000; i++)
		{
			j = j + 1;
		}

	}

    c = a + b;
    printf ("a+b is %i\n", c);
    c = a - b;
    printf ("a-b is %i\n", c);
    c = a * b;
    printf ("a*b is %i\n", c);
    c = a / b;
    printf ("a/b is %i\n", c);

	b = 10;
    c = a / b;
    printf ("a/b is %i\n", c);

  	while (1)

    ;
}

//  test function
void f1(int y)
{
	y = y+1;
    return;
}