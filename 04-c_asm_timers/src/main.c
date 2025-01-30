#include <stdio.h>
#include <mos_api.h>

#define TMR5_CTL  0x008F		// Timer 5 control register
#define TMR5_DR_L 0x0090		// Timer 5 data-read (LSB) --> read this first
#define TMR5_DR_H 0x0091		// Timer 5 data-read (MSB) --> read this after LSB
#define TMR5_RR_L 0x0090		// Timer 5 reload register (LSB)
#define TMR5_RR_H 0x0091		// Timer 5 reload register (MSB)

void timerTestSetup();
extern unsigned int upperTimer5Counter;

int main()
{
	printf("Before calling _timerTestSetup\n");
	timerTestSetup();						// Calling from C to assembly code ('timertest.asm')
	printf("After calling _timerTestSetup\n");
	
	int i = 0;
	for(i = 0; i < 24; i++)
	{
		unsigned char l = mos_port_read(TMR5_DR_L);
		unsigned char h = mos_port_read(TMR5_DR_H);
		unsigned int  u = upperTimer5Counter;
		
		printf("%d) timer5: %4.4x%2.2x%2.2x\n", i, u, h, l);	
	}	
	
	return 1;
}
