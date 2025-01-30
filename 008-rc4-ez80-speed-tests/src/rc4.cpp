/****************************************************************
	2024/08/24 19:08 NZT
	RC4 software implementation

	* (following implementation from the book "" and my previous
	    implementation adapted to Microsoft C#)
 	* Translated into Zilog eZ80 CEdev/AgDev C/C++ environment and data types
 	    for Agon Light 2.

*****************************************************************/
#include <string.h>
#include "rc4.h"
#include "numtools.h"

#define DEBUG_MODE 0


byte* rc4_crypt(byte *msgArr, byte* keyArr, byte* outputBuffer, int msgLen, int keyLength)
{
	byte temp;
	int i, j;
	byte S[256];		// Internal state array for this encryption algorithm.

	// Initialise S array with values from 0 to 255.
	for (i = 0; i <= 255; i++)
		S[i] = (byte)i;


	// Apply Key Scheduling Algorithm (KSA) over the S array.
	for (i = 0, j = 0; i <= 255; i++)
	{
		j = (j + S[i] + keyArr[i % keyLength]) % 256;
		temp = S[i];
		S[i] = S[j];
		S[j] = temp;
	}

	// Generate a keystream KS to be exclusive-ored with the original message msg.
	i = 0;
	j = 0;
	for(int b = 0; b < msgLen; b++)
	{
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		temp = S[i];
		S[i] = S[j];
		S[j] = temp;
		byte ks = S[(S[i] + S[j]) % 256];

		outputBuffer[b] = (byte)(ks ^ msgArr[b]);
	}

	// Returns the same initial output byte array to caller function.
	return outputBuffer;
}
