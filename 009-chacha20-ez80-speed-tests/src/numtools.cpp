#include <stdio.h>
#include <string.h>
#include <mos_api.h>
#include "numtools.h"

char nmtmp0[64], nmtmp1[128], nmtmp2[256];

/*
#define TMR3_CTL  0x0089		// Timer 3 control register
#define TMR3_DR_L 0x008A		// Timer 3 data-read (LSB) --> read this first
#define TMR3_DR_H 0x008B		// Timer 3 data-read (MSB) --> read this after LSB
#define TMR3_RR_L 0x008A		// Timer 3 reload register (LSB)
#define TMR3_RR_H 0x008B		// Timer 3 reload register (MSB)
*/

byte TMR5_COUNTER_UPPER;		// Upper 8-bits for TIMER 5 counter (based on software, not on interrupts or hardware)

#define TMR5_CTL  0x008F		// Timer 5 control register
#define TMR5_DR_L 0x0090		// Timer 5 data-read (LSB) --> read this first
#define TMR5_DR_H 0x0091		// Timer 5 data-read (MSB) --> read this after LSB
#define TMR5_RR_L 0x0090		// Timer 5 reload register (LSB)
#define TMR5_RR_H 0x0091		// Timer 5 reload register (MSB)

void  micros_setup()
{
	// Configures Timer 5 to count from 0xffff to 0x0000.
	//   It increments TMR5_COUNTER_UPPER each time the count resets to 0xffff again and again.
	//   1/256 scaler @ 18.432 [MHz] ---> 13.88 [us] ~ 14 [us] for each decrement (resolution) 
	//                               ---> lasts 910.15 [ms] between each value reloading.
	//
	//	*With UPPER byte (16-bits), this counter lasts for 59,647 [s] ~ 994.127 [min] ~ 16.57 [h], approx.

	mos_port_write(TMR5_CTL, 0x00 | 0x08 | 0x04);	// disable IRQ, single-mode, 1/256 scaler, reload, disable the Timer 5.
	mos_port_write(TMR5_RR_L, 0xff);				// Reload 0xffff after reaching 0x0000 and generate an IRQ.
	mos_port_write(TMR5_RR_H, 0xff);

	TMR5_COUNTER_UPPER = 0xff;					// Holds bits 23-16 of this counter (Timer 5).	
	//mos_setintvector(0x10, incrUpperMicros);
	mos_port_write(TMR5_CTL, 0x10 | 0x08 | 0x04 | 0x02 | 0x01);	// Reload & enable the timer 5 (counting downwards).
}

void  micros_stop()
{
	mos_port_write(TMR5_CTL, 0x00);
}

// micros() returns a number of ticks measured in TIMER 5 (TMR5).
// Each tick in TIMER5 is (18,432,000 [MHz])^-1 = 13.888 [us] ~ 14 [us]
uint micros()
{
	byte l = mos_port_read(TMR5_DR_L);
	byte h = mos_port_read(TMR5_DR_H);
	updateMicros();

	//mos_port_write(TMR5_CTL, 0x00);
	//mos_port_write(TMR5_CTL, 0x08 | 0x04 | 0x02 | 0x01);
	//mos_port_read(TMR5_CTL);

	return (TMR5_COUNTER_UPPER << 16) | (h << 8) | l;
}

void updateMicros()
{
	if(mos_port_read(TMR5_CTL) & 0x80)
	{
		//mos_port_read(TMR5_CTL);				// Read and clear TMR5_OVERFLOW bit (bit 7).
		TMR5_COUNTER_UPPER--;
	}
}


char NumTools_toHexDigit(byte c)
{
	c = c & 0x0f;

	if (c >= 0 && c <= 9)
		return (char)(c + '0');
	else
		return (char)(c + 'a' - 10);
}


char* NumTools_toHex(ulong d, byte maxNumOfHexDigits)
{
	if(maxNumOfHexDigits == 0)
		maxNumOfHexDigits = 1;

	if(maxNumOfHexDigits > 8) 
		maxNumOfHexDigits = 8;

	nmtmp0[8] = '\0';

	int ci = 8;
	for (int i = 1; i <= maxNumOfHexDigits; i++)
	{
		ci--;
		nmtmp0[ci] = NumTools_toHexDigit(d & 0x0f);
		d >>= 4;
	}

	return (nmtmp0 + ci);
}

char* NumTools_toHex2(byte d)
{
	return NumTools_toHex((ulong)d, 2);
}

char* NumTools_toHex4(uint d)
{
	return NumTools_toHex((ulong)d, 4);
}

char* NumTools_toHex8(ulong d)
{  
	return NumTools_toHex(d, 8);
}

char* NumTools_toHex8Reverse(ulong d)
{
	char *ph = NumTools_toHex8(d);

	int j = 0;
	for(int i = strlen(ph) - 1; i >= 0; i--, j++)
	{
		nmtmp1[j] = ph[i];
	}
	
	nmtmp1[j] = '\0';

	return nmtmp1;
}

char* NumTools_toHex8Reverse_2b(ulong d)
{
	char *ph = NumTools_toHex8(d);

	nmtmp1[0] = ph[6];	nmtmp1[1] = ph[7];
	nmtmp1[2] = ph[4];	nmtmp1[3] = ph[5];
	nmtmp1[4] = ph[2];	nmtmp1[5] = ph[3];
	nmtmp1[6] = ph[0];	nmtmp1[7] = ph[1];
	nmtmp1[8] = '\0';

	return nmtmp1;
}

/*char* asciiToHex2Str(string msg, bool addOneSpaceBetween = false)
        {
            string outstr = "";

            for (int i = 0; i < msg.Length; i++)
            {
                outstr += NumTools.toHex2((byte)msg[i]);
                if (addOneSpaceBetween)
                    outstr += " ";
            }

            return outstr;
        }
*/


void NumTools_printArray_tui(char *title, ulong arr[], uint maxElements, byte cols, bool asHex, byte hexDigits)
{
	printf("%s\n", title);
	printf("===============================\n");

	NumTools_printArray_ul(arr, maxElements, cols, asHex, hexDigits);
}


void NumTools_printArray_b(byte *arr, int maxElements, byte cols, bool asHex, bool inoneline, bool withSpaces)
{
	int i = 0, j = 0;
	while (i < maxElements)
	{
		if (asHex)
		{
			//printf("%s", NumTools_toHex2(arr[i++]));
			printf("%2.2x", arr[i++]);
			if(withSpaces)
				printf(" ");
		}
		else
		{
			printf("%d ", arr[i++]);
		}

		if (!inoneline && ++j >= cols)
		{
			printf("\n");
			j = 0;
		}
	}
}


void NumTools_printArray_bf(FILE *f, byte *arr, int maxElements, byte cols, bool asHex, bool inoneline, bool withSpaces)
{
	int i = 0, j = 0;
	while (i < maxElements)
	{
		if (asHex)
		{
			//fprintf(f, "%s", NumTools_toHex2(arr[i++]));
			fprintf(f, "%2.2x", arr[i++]);
			if(withSpaces)
				fprintf(f, " ");
		}
		else
		{
			fprintf(f, "%d ", arr[i++]);
		}

		if (!inoneline && ++j >= cols)
		{
			fprintf(f, "\n");
			j = 0;
		}
	}
}


void NumTools_printArray_ui(uint *arr, int maxElements, byte cols, bool asHex, byte hexDigits)
{
	int i = 0, j = 0;
	while (i < maxElements)
	{
		if (asHex)
		{
			switch(hexDigits)
			{
				case 2:  printf("%s", NumTools_toHex2(arr[i++])); break;
				case 4:  printf("%s", NumTools_toHex4(arr[i++])); break;
				default: printf("%s", NumTools_toHex8(arr[i++])); break;
			}

      		printf(" ");
		}
		else
    		{	
			printf("%lu ", arr[i++]);
    		}

		if (++j >= cols)
		{
			printf("\n");
			j = 0;
		}
	}

	printf("\n");
}


void NumTools_printArray_ul(ulong *arr, int maxElements, byte cols, bool asHex, byte hexDigits)
{
	int i = 0, j = 0;
	while (i < maxElements)
	{
		if (asHex)
		{
			switch(hexDigits)
			{
				case 2:  printf("%s", NumTools_toHex2(arr[i++])); break;
				case 4:  printf("%s", NumTools_toHex4(arr[i++])); break;
				default: printf("%s", NumTools_toHex8(arr[i++])); break;
			}

      		printf(" ");
		}
		else
    		{
			printf("%llu ", arr[i++]);
    		}

		if (++j >= cols)
		{
			printf("\n");
			j = 0;
		}
	}

	printf("\n");
}

