/****************************************************************
	2024/08/24 18:32 NZT
	SHA-1 software implementation

	* (following implementation from the book "" and my previous
	    implementation adapted to Microsoft C#)
 	* Translated into Zilog eZ80 CEdev/AgDev C/C++ environment and data types
 	    for Agon Light 2.
*****************************************************************/
#include <string.h>
#include "sha1.h"
#include "numtools.h"

#define DEBUG_MODE 0


ulong   A, B, C, D, E, aa, bb, cc, dd, ee;
ulong   paddingRemainingBytes;
bool    usePadding, paddingFirstByte, lastBlock;
char    finalHashArr[40];


char* sha1_hash(byte *arrData, long totalDataLen)
{
	ulong M[16], W[80];
	byte block64[64];

	lastBlock = false;
	usePadding = (totalDataLen /*arrData.Length*/ > 0? false: true);      // If input data is an empty string, just add a padding for the process.
	paddingFirstByte = (totalDataLen /*arrData.Length*/ > 0? false : true); 
	paddingRemainingBytes = 0;


	// Initialise internal state for MD5 calculations.
	A = 0x67452301UL;	//0x01234567;        // 4 'chaining' variables.
	B = 0xefcdab89UL;	// 0x89abcdef;
	C = 0x98badcfeUL;	// 0xfedcba98;
	D = 0x10325476UL;	// 0x76543210;
  	E = 0xc3d2e1f0UL;

/*	if(DEBUG_MODE >= 3)
  {
  	Serial.print("A=");
    Serial.print(A, HEX);
    Serial.print("  B=");
    Serial.print(B, HEX);
    Serial.print("  C=");
    Serial.print(C, HEX);
    Serial.print("  D=");
    Serial.print(D, HEX);
    Serial.print("  E=");
    Serial.println(E, HEX);
  }
*/		

	int i = 0;

	while(lastBlock == false)
	{
		for (int j = 0; j < 64; j++)
		{
			updateMicros();
		
			if (usePadding == false)
			{
				block64[j] = arrData[i++];
				if(i == totalDataLen)
				{
					usePadding = true;
					paddingFirstByte = true;
					paddingRemainingBytes = (j < 55? 55-j: 120-j-1);

					/*if(DEBUG_MODE >= 3)
					{
						Serial.print(F("PaddingRemainingBytes = "));
						Serial.println(paddingRemainingBytes);
					}*/
				}
			}
			else
			{
				if(paddingFirstByte == true)
				{
					paddingFirstByte = false;
					block64[j] = 0x80;
					paddingRemainingBytes--;
				}
				else
				{
					if (paddingRemainingBytes > 0)
					{
						block64[j] = 0x00;
						paddingRemainingBytes--;
					}
					else
					{
						/*if (DEBUG_MODE >= 3)
							Serial.println("lastblock");*/

						lastBlock = true;

						// Add total data length (before padding) to the end of this last block.
						totalDataLen *= 8;      // Length in bytes becomes length in bits.

						block64[63] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[62] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[61] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[60] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[59] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[58] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[57] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[56] = (byte)(totalDataLen & 0xff);

						break;
					}
				}
			}
		}

    /*if (DEBUG_MODE >= 3)
    {
        Serial.println(F("Block16:"));
        NumTools_printArray_b(block64, 64, 16, true);
    }*/

    // Pack 4 contiguous bytes (four 8-bits, in LSB format) into one M array cell (with 32-bits, in LSB format)
    for (int j = 0, k = 0; j < 16; j++, k += 4)         // indexes 0->13: data or padding / indexes 14->15: total data length
    {
        M[j]  = (ulong)block64[k + 3];
        M[j] |= ((ulong)block64[k + 2]) << 8;
        M[j] |= ((ulong)block64[k + 1]) << 16;
        M[j] |= ((ulong)block64[k]) << 24;

        W[j] = M[j];
    }

    // Now, transform 16 32-bits words from W[0] to W[15] into 80 32-bits words
		//   inside W[16] to W[79]
		for(int t = 16; t <= 79; t++)
		{
			W[t] = rol(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
		}

    /*if(DEBUG_MODE >= 2)
        NumTools_printArray_ui(M, 16, 16, true, 8);*/

    sha1_calculate(W);
      
    /*if(DEBUG_MODE >= 3)
    {
      Serial.print("A=");
      Serial.print(A, HEX);
      Serial.print("  B=");
      Serial.print(B, HEX);
      Serial.print("  C=");
      Serial.print(C, HEX);
      Serial.print("  D=");
      Serial.print(D, HEX);
      Serial.print("  E=");
      Serial.print(E, HEX);
      Serial.println();
    }*/
    //    Serial.println("A=%8.8x  B=%8.8x  C=%8.8x  D=%8.8x\r\r", A, B, C, D);*/
  }

  strcpy(finalHashArr, NumTools_toHex8(A));
  strcat(finalHashArr, NumTools_toHex8(B));
  strcat(finalHashArr, NumTools_toHex8(C));
  strcat(finalHashArr, NumTools_toHex8(D));
  strcat(finalHashArr, NumTools_toHex8(E));

  return finalHashArr;
}


void sha1_calculate(ulong *W)
{
	aa = A;                 // 4 accumulators of 32-bits length.
	bb = B;
	cc = C;
	dd = D;
	ee = E;

	for(byte t = 0; t <= 79; t++)
	{
		ulong temp = rol(aa, 5) + Ft(bb, cc, dd, t) + ee + W[t] + Kt(t);
		ee = dd;
		dd = cc;
		cc = rol(bb, 30);
		bb = aa;
		aa = temp;
	}

	// Final sum and accumulation.
	A += aa;
	B += bb;
	C += cc;
	D += dd;
	E += ee;

	/*if (DEBUG_MODE >= 4)
  {
		Serial.print("accum: aa=");
    Serial.print(aa, HEX);
    Serial.print("  bb=");
    Serial.print(bb, HEX);
    Serial.print("  cc=");
    Serial.print(cc, HEX);
    Serial.print("  dd=");
    Serial.println(dd, HEX);
  }*/
	//	printf("accum: aa=%8.8x  bb=%8.8x  cc=%8.8x  dd=%8.8x\r\r", aa, bb, cc, dd);

	// Go above and apply all these steps over the next byte block, until reaching last block (with padding included).
}


//---------------------------------------------------------------
// Implements ROL (rotate-on-left) bit function over 32-bits unsigned data.
ulong rol(ulong v, int times)
{
	bool carry = false;

	for(int i = 1; i <= times; i++)
	{
		carry = (ulong)((v & 0x80000000UL) != 0UL);
		v <<= 1;
		v |= (ulong)(carry? 1UL: 0UL);
	}

	return v;
}

//---------------------------------------------------------------
ulong Ft(ulong x, ulong y, ulong z, byte t)
{
	if (t >= 0 && t <= 19)
		return (x & y) | ((~x) & z);
	else if (t >= 20 && t <= 39)
		return x ^ y ^ z;
	else if (t >= 40 && t <= 59)
		return (x & y) | (x & z) | (y & z);
	else //if (t >= 60 && t <= 79)
		return x ^ y ^ z;
}


//---------------------------------------------------------------
ulong Kt(byte t)
{
	if (t >= 0 && t <= 19)
		return 0x5a827999UL;
	else if (t >= 20 && t <= 39)
		return 0x6ed9eba1UL;
	else if (t >= 40 && t <= 59)
		return 0x8f1bbcdcUL;
	else 					//if (t >= 60 && t <= 79)
		return 0xca62c1d6UL;
}
