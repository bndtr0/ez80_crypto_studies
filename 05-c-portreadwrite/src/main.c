#include <stdio.h>
#include <mos_api.h>

typedef char          boolean;
typedef unsigned char byte;
typedef unsigned int  uint;
typedef unsigned long ulong;

void printVersion();
void printHelp();
void printBin8(byte num, boolean printSpaces, boolean newLineAtTheEnd);
void usdelay(ulong v);
void msdelay(ulong v);

int main()
{
	char c1, c2;
	uint pval, dval, nval;
	int cols;
	
	printHelp();
	
	while(1)
	{
		printf(">");
		scanf("%c", &c1);
		
		if(c1 == 'q' || c1 == 'Q')
			break;
			
		if(c1 == 'h' || c1 == 'H')
			printHelp();
			
		if(c1 == 'v' || c1 == 'V')
			printVersion();
			
		if(c1 == 'd')
		{
			for(int i = 1; i < 20; i++)
			{
				printf("<");
				msdelay(500);
				printf(">");
			}
			printf("\n");
		}
		
		if(c1 == 'p' || c1 == 'P')
		{	
			scanf("%c", &c2);
			
			if(c2 == 'r' || c2 == 'R')
			{
				scanf("%x", &pval);
				
				printf("port(%2.2x) -> %2.2x\n", pval, mos_port_read(pval));
			}
		
			if(c2 == 'b' || c2 == 'B')
			{
				scanf("%x", &pval);
				scanf("%x", &dval);
				
				printf("ports(%2.2x:%2.2x)\n\n", pval, pval + dval - 1);
				printf("----------------\n");
				
				cols = 0;
				for(uint i = pval; i < pval + dval; i++)
				{
					if(cols == 0)
						printf("0x%4.4x: ", i);
						
					printf("%2.2x ", mos_port_read(i));
					cols++;
					
					if(cols > 7)
					{
						cols = 0;						
						printf("\n");
					}
				}
					
				printf("\n");
			}
		
			if(c2 == 'w' || c2 == 'W')
			{
				scanf("%x", &pval);
				scanf("%x", &dval);
				
				printf("port(%2.2x) <- %2.2x\n", pval, dval);
				mos_port_write(pval, dval);
				
				printf("\n");
			}
			
			
			if(c2 == 't' || c2 == 'T')
			{
				scanf("%x", &pval);
				scanf("%x", &dval);
				scanf("%d", &nval);
				
				printf("Read IN ports %d times:", nval);
				
				for(uint ntimes = 1; ntimes <= nval; ntimes++)
				{
					printf("(%5.5d) port(0x%2.2x): ", ntimes, pval);
					
					cols = 0;
					for(uint i = pval; i < pval + dval; i++)
					{
						/*if(cols == 0)
							printf("0x%2.2x: ", i);*/
							
						printf("%2.2x ", mos_port_read(i));
						cols++;
						
						/*if(cols > 15)
						{
							cols = 0;						
							printf("\n");
						}*/
					}
						
					printf("\n");
				}
			}
			
			
			
		}
		
		
		if(c1 == 'm' || c1 == 'M')
		{		
			scanf("%c", &c2);
			
			if(c2 == 'r' || c2 == 'R')
			{
				scanf("%x", &pval);
				
				printf("mem(%6.6x) -> %2.2x\n", pval, (unsigned char)*((unsigned char *)pval));
			}
			
			if(c2 == 'w' || c2 == 'W')
			{
				scanf("%x", &pval);
				scanf("%x", &dval);
				
				printf("mem(%6.6x) <- %2.2x\n", pval, dval);
				*((unsigned char *)pval) = dval;
				
				printf("\n");
			}
			
			if(c2 == 'b' || c2 == 'B')
			{
				scanf("%x", &pval);
				scanf("%x", &dval);
				
				printf("memb(%6.6x:%6.6x):\n\n", pval, pval + dval - 1);
				printf("--------------------\n");
				
				cols = 0;
				for(uint i = pval; i < pval + dval; i++)
				{
					if(cols == 0)
						printf("0x%6.6x: ", i);
						
					printf("%2.2x ", (unsigned char)*((unsigned char *)i));
					cols++;
					
					if(cols > 7)
					{
						cols = 0;
						printf("     ");
						
						for(uint j = i - 7; j <= i; j++)
						{
							byte c = (unsigned char)*((unsigned char *)j);
							printf("%c ", (c >= 32? c: '.'));
						}						
						printf("\n");
					}
				}
					
				printf("\n");
			}
		}
		
	}
	
	return 0;
}




void printVersion()
{
	printf("\n");
	printf("+------------------------+\n");
	printf("| ZiLog eZ80F92 CPU      |\n");
	printf("| port-mem-reader-writer |\n");
	printf("+------------------------+\n");
	printf("2024-08-23 19:10\n");
	printf("Version 0.1\n");
	printf("Tool for analysis of SRAM and HW ports in Zilog eZ80F92 CPU.\n\n");
}

void printHelp()
{
	printf("\n");
	printf("+------------------------+\n");
	printf("| ZiLog eZ80F92 CPU      |\n");
	printf("| port-mem-reader-writer |\n");
	printf("+------------------------+\n");
	printf("pr <paddr>            : read IN port\n");
	printf("pb <paddr> <count>    : read a block of IN ports ('count' addresses)\n");
	printf("pw <paddr> <value>    : write value in OUT port\n");
	printf("--------------------------\n");
	printf("mr <maddr>            : read SRAM value from a SRAM address\n");
	printf("mw <maddr> <value>    : write SRAM value into a SRAM address\n");
	printf("mb <maddr> <count>    : see SRAM values inside a block\n");
	printf("--------------------------\n");
	printf("pt <paddr> <count> <n>: read a block of <count> IN ports <n> times\n");
	printf("--------------------------\n");
	printf("h                     : shows this help text\n");
	printf("q                     : quit program\n\n");
}


void printBin8(byte num, boolean printSpaces, boolean newLineAtTheEnd)
{
	printf("%%");
	
	for(byte b = 0; b <= 7; b++)
	{
		printf("%d", (num & 0x80? 1: 0));
		num <<= 1;
		if(printSpaces)
			printf(" ");
	}

	if(newLineAtTheEnd)
		printf("\n");	
}


void usdelay(ulong v)
{
	asm("push bc");
	
	for(; v > 0UL; v--)
	{
		asm("ld bc, 101010h");	// 5 cycles 
		asm("ld bc, 101010h");
		asm("ld bc, 101010h");
		asm("ld bc, 101010h");
		asm("ld bc, 101010h");
		asm("ld bc, 101010h");
		asm("ld bc, 101010h");
		asm("ld bc, 101010h");
		asm("ld bc, 101010h");
		asm("ld bc, 101010h");	// total --> 50 cycles --> 1 [us] @ 50 [MHz]
	}
	
	asm("pop bc");
}


void msdelay(ulong v)
{
	asm("push bc");
	
	for(; v > 0UL; v--)
	{
		for(int i = 1; i <= 1000; i++)	// -----> 50 cycles * 1000 --> 1 [us] * 1000 --> ~1 [ms]
		{
			asm("ld bc, 101010h");		// 5 cycles --> 100 [ns] @ 50 [MHz]
			asm("ld bc, 101010h");
			asm("ld bc, 101010h");
			asm("ld bc, 101010h");
			asm("ld bc, 101010h");
			asm("ld bc, 101010h");
			asm("ld bc, 101010h");
			asm("ld bc, 101010h");
			asm("ld bc, 101010h");
			asm("ld bc, 101010h");		// total --> 50 cycles --> 1 [us] @ 50 [MHz]
		}
	}
	
	asm("pop bc");
}
