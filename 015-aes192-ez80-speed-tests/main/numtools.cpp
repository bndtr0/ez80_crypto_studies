#include <Arduino.h>
#include <string.h>
#include "numtools.h"

char nmtmp0[64], nmtmp1[128], nmtmp2[256];


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
	Serial.println(title);
	Serial.println(F("==============================="));

	NumTools_printArray_ui(arr, maxElements, cols, asHex, hexDigits);
}


void NumTools_printArray_b(byte *arr, int maxElements, byte cols, bool asHex)
{
	int i = 0, j = 0;
	while (i < maxElements)
	{
		if (asHex)
		{
			Serial.print(NumTools_toHex2(arr[i++]));
		}
		else
    {
			Serial.print(arr[i++]);
      Serial.print(" ");
    }

		if (++j >= cols)
		{
			Serial.println();
			j = 0;
		}
	}

	Serial.println();
}


void NumTools_printArray_ui(ulong *arr, int maxElements, byte cols, bool asHex, byte hexDigits)
{
	int i = 0, j = 0;
	while (i < maxElements)
	{
		if (asHex)
		{
			switch(hexDigits)
			{
				case 2:  Serial.print(NumTools_toHex2(arr[i++])); break;
				case 4:  Serial.print(NumTools_toHex4(arr[i++])); break;
				default: Serial.print(NumTools_toHex8(arr[i++])); break;
			}

      Serial.print(" ");
		}
		else
    {
			Serial.print(arr[i++]);
      Serial.print(" ");
    }

		if (++j >= cols)
		{
			Serial.println();
			j = 0;
		}
	}

	Serial.println();
}

