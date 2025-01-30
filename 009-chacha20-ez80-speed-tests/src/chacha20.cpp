/****************************************************************
	2024/09/14 18:40 NZT
	ChaCha20 software implementation

    Implementation of ChaCha20, as suggested in RFC7535 (after Daniel J. Bernstein).
        * Nonce word increased to 96-bits.
        * Counter field reduced to 32-bits (2^32 blocks of 64 bytes each = 256 GiB)
        * Conversion from Raspberry Pi Pico SDK to Zilog eZ80 SDK.

*****************************************************************/
#include <stdio.h>
#include <string.h>
#include "chacha20.h"
#include "numtools.h"

#define DEBUG_MODE 0


byte* chacha20_crypt(char *msg, char *key, byte *nonce, uint counter, byte* outputBuffer,
					 uint msgLen, uint keyLen, boolean useThisCounter)
{
	if(useThisCounter)
		return chacha20_calculate((byte*)msg, (byte*)key, nonce, counter, outputBuffer, msgLen);
	else
		return chacha20_calculate((byte*)msg, (byte*)key, nonce, 1, outputBuffer, msgLen);
}



byte* chacha20_calculate(byte *msgArr, byte* keyArr, byte* nonceArr, uint counter, 
					byte* outputBuffer, int msgLen)
{
	uint M[16], S[16];
	//uint il = (uint)input.Length;

	for (int j = 0; j < msgLen; j += 64)
	{
		updateMicros();

		// Initialise state matrix
		M[0]  = LSB_s4((char*)"expa");
		M[1]  = LSB_s4((char*)"nd 3");
		M[2]  = LSB_s4((char*)"2-by");
		M[3]  = LSB_s4((char*)"te k");

		M[4]  = LSB(keyArr[0], keyArr[1], keyArr[2], keyArr[3]);			// Key: first 4 32-bits words in LSB order
		M[5]  = LSB(keyArr[4], keyArr[5], keyArr[6], keyArr[7]);
		M[6]  = LSB(keyArr[8], keyArr[9], keyArr[10], keyArr[11]);
		M[7]  = LSB(keyArr[12], keyArr[13], keyArr[14], keyArr[15]);

		M[8]  = LSB(keyArr[16], keyArr[17], keyArr[18], keyArr[19]);		// Key: last 4 32-bits words in LSB order
		M[9]  = LSB(keyArr[20], keyArr[21], keyArr[22], keyArr[23]);
		M[10] = LSB(keyArr[24], keyArr[25], keyArr[26], keyArr[27]);
		M[11] = LSB(keyArr[28], keyArr[29], keyArr[30], keyArr[31]);

		M[12] = counter++;                                      			// Counter 32-bits (initial number)
		M[13] = LSB(nonceArr[0], nonceArr[1], nonceArr[2], nonceArr[3]);		// Nonce 96-bits long
		M[14] = LSB(nonceArr[4], nonceArr[5], nonceArr[6], nonceArr[7]);
		M[15] = LSB(nonceArr[8], nonceArr[9], nonceArr[10], nonceArr[11]);

		//arrayClear(ref S);
		arrayCopy(S, M);

		// Apply 10 rounds to the state array
		for (int i = 1; i <= 10; i++)
		{
			QuarterRound(M, 0, 4,  8, 12);
			QuarterRound(M, 1, 5,  9, 13);
			QuarterRound(M, 2, 6, 10, 14);
			QuarterRound(M, 3, 7, 11, 15);
			QuarterRound(M, 0, 5, 10, 15);
			QuarterRound(M, 1, 6, 11, 12);
			QuarterRound(M, 2, 7,  8, 13);
			QuarterRound(M, 3, 4,  9, 14);
		}

		// Sum the final state array (after 10 rounds) to initial state array
		/*printf("State matrix (initial S):\r");
		NumTools_printArray_ui(S, 16, 4, true, false); 
		getchar();

		printf("State matrix (block M)  :\r");
		NumTools_printArray_ui(M, 16, 4, true, false);
		getchar();*/

		arraySum(S, M);

		/*printf("State matrix (S = S + M):\r");
		NumTools_printArray_ui(S, 16, 4, true, false);
		getchar();

		printf("key:\r");*/
		for (int i = 0, k = 0; i < 64; i += 4, k++)
		{
			uint kk = S[k];
			byte k0 = (byte)(kk & 0xff); kk >>= 8;
			byte k1 = (byte)(kk & 0xff); kk >>= 8;
			byte k2 = (byte)(kk & 0xff); kk >>= 8;
			byte k3 = (byte)(kk & 0xff);

			/*printf("[k0,k1,k2,k3] => [%2.2x:%2.2x:%2.2x:%2.2x]\r", k0, k1, k2, k3);
			getchar();*/

			if (i + j >= msgLen) break;
			//output += NumTools.toHex2((byte)(input[i + j] ^ k0)) + " ";
			outputBuffer[i + j] = (byte)(msgArr[i + j] ^ k0);

			if (i + j + 1 >= msgLen) break;
			//output += NumTools.toHex2((byte)(input[i + j + 1] ^ k1)) + " ";
			outputBuffer[i + j + 1] = (byte)(msgArr[i + j + 1] ^ k1);

			if (i + j + 2 >= msgLen) break;
			//output += NumTools.toHex2((byte)(input[i + j + 2] ^ k2)) + " ";
			outputBuffer[i + j + 2] = (byte)(msgArr[i + j + 2] ^ k2);

			if (i + j + 3 >= msgLen) break;
			//output += NumTools.toHex2((byte)(input[i + j + 3] ^ k3)) + " ";
			outputBuffer[i + j + 3] = (byte)(msgArr[i + j + 3] ^ k3);
		}
	}

	//printf("\r\r");
	
	// Returns the same initial output byte array to caller function.
	return outputBuffer;
}

uint rol(uint v, uint times)
{
	boolean carry = false;

	for (uint i = 1; i <= times; i++)
	{
		carry = ((v & 0x80000000) != 0UL);
		v <<= 1;
		v |= (carry ? 1UL : 0UL);
	}

	return v;
}


uint LSB(byte a, byte b, byte c, byte d)
{
	/*ulong r = a;
	r |= (ulong)((ulong)b << 8);
	r |= (ulong)((ulong)c << 16);
	r |= (ulong)((ulong)d << 24);*/
	uint r = d;
	r <<= 8;
	r |= c;
	r <<= 8;
	r |= b;
	r <<= 8;
	r |= a;

	return r;
}

uint LSB_s4(char *s)
{
	/*ulong r = s[0];
	r |= (ulong)((ulong)s[1] << 8);
	r |= (ulong)((ulong)s[2] << 16);
	r |= (ulong)((ulong)s[3] << 24);*/
	uint r = s[3];
	r <<= 8;
	r |= s[2];
	r <<= 8;
	r |= s[1];
	r <<= 8;
	r |= s[0];

	return r;
}

// ChaCha20's Quarterround definition
void QuarterRound(uint *M, int ia, int ib, int ic, int id)
{
	M[ia] += M[ib]; M[id] ^= M[ia]; M[id] = rol(M[id], 16);
	M[ic] += M[id]; M[ib] ^= M[ic]; M[ib] = rol(M[ib], 12);
	M[ia] += M[ib]; M[id] ^= M[ia]; M[id] = rol(M[id], 8);
	M[ic] += M[id]; M[ib] ^= M[ic]; M[ib] = rol(M[ib], 7);
}

void arrayClear(uint *arrd)
{
	for (int i = 0; i <= 15; i++)
		arrd[i] = 0;
}

void arrayCopy(uint *arrd, uint *arrs)
{
	for (int i = 0; i <= 15; i++)
		arrd[i] = arrs[i];
}

void arraySum(uint *arrd, uint *arrs)
{
	for (int i = 0; i <= 15; i++)
		arrd[i] += arrs[i];
}

