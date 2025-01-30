/****************************************************************
	2024/08/24 18:49 NZT
	SHA-256 software implementation

	* (following implementation from the book "" and my previous
	    implementation adapted to Microsoft C#)
 	* Translated into Zilog eZ80 CEdev/AgDev C/C++ environment and data types
 	    for Agon Light 2.
*****************************************************************/
#include <string.h>
#include "sha256.h"
#include "numtools.h"

#define DEBUG_MODE 0


ulong   h[8], aa, bb, cc, dd, ee, ff, gg, hh;
ulong   M[16], W[64];
byte    block64[64];
ulong   paddingRemainingBytes;
bool    usePadding, paddingFirstByte, lastBlock;
char    finalHashArr[64];

ulong k[64] = {
   0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
   0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
   0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL, 0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
   0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
   0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
   0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
   0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
   0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL, 0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL };


char* sha256_hash(byte *arrData, long totalDataLen)
{
	lastBlock = false;
	usePadding = (totalDataLen /*arrData.Length*/ > 0? false: true);      // If input data is an empty string, just add a padding for the process.
	paddingFirstByte = (totalDataLen /*arrData.Length*/ > 0? false : true); 
	paddingRemainingBytes = 0;

	// Initialise internal state for SHA-256 calculations.
	h[0] = 0x6a09e667UL;
	h[1] = 0xbb67ae85UL;
	h[2] = 0x3c6ef372UL;
	h[3] = 0xa54ff53aUL;
	h[4] = 0x510e527fUL;
	h[5] = 0x9b05688cUL;
	h[6] = 0x1f83d9abUL;
	h[7] = 0x5be0cd19UL;

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
    Serial.println(E, HEX);
  }*/
		

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

    // Now, transform 16 32-bits words from W[0] to W[15] into 64 32-bits words
		//   inside W[16] to W[63]
	ulong s0, s1;
	
	for(int t = 16; t <= 63; t++)
	{
		s0   = ror(W[t-15], 7) ^ ror(W[t-15], 18) ^ (W[t-15] >> 3);
		s1   = ror(W[t-2], 17) ^ ror(W[t-2], 19) ^ (W[t-2] >> 10);
		W[t] = W[t-16] + s0 + W[t-7] + s1;
	}

    /*if(DEBUG_MODE >= 2)
        NumTools_printArray_ui(M, 16, 16, true, 8);*/

    sha256_calculate(W);
      
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

	strcpy(finalHashArr, NumTools_toHex8(h[0]));
	strcat(finalHashArr, NumTools_toHex8(h[1]));
	strcat(finalHashArr, NumTools_toHex8(h[2]));
	strcat(finalHashArr, NumTools_toHex8(h[3]));
	strcat(finalHashArr, NumTools_toHex8(h[4]));
	strcat(finalHashArr, NumTools_toHex8(h[5]));
	strcat(finalHashArr, NumTools_toHex8(h[6]));
	strcat(finalHashArr, NumTools_toHex8(h[7]));

	return finalHashArr;
}


void sha256_calculate(ulong *W)
{
	aa = h[0];      // 8 accumulators of 32-bits length.
	bb = h[1];
	cc = h[2];
	dd = h[3];
	ee = h[4];
	ff = h[5];
	gg = h[6];
	hh = h[7];

	ulong s0, s1, ch, maj, temp1, temp2;
	
	for(byte t = 0; t <= 63; t++)
	{
		s1    = ror(ee, 6) ^ ror(ee, 11) ^ ror(ee, 25);
		ch    = (ee & ff) ^ ((~ee) & gg);
		temp1 = hh + s1 + ch + k[t] + W[t];
		s0    = ror(aa, 2) ^ ror(aa, 13) ^ ror(aa, 22);
		maj   = (aa & bb) ^ (aa & cc) ^ (bb & cc);
		temp2 = s0 + maj;

		hh = gg;
		gg = ff;
		ff = ee;
		ee = dd + temp1;
		dd = cc;
		cc = bb;
		bb = aa;
		aa = temp1 + temp2;
	}

	// Final sum and accumulation.
	h[0] += aa;
	h[1] += bb;
	h[2] += cc;
	h[3] += dd;
	h[4] += ee;
	h[5] += ff;
	h[6] += gg;
	h[7] += hh;

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
// Implements ROR (rotate-on-right) bit function over 32-bits unsigned data.
ulong ror(ulong v, int times)
{
	bool carry = false;

	for(int i = 1; i <= times; i++)
	{
		carry = (ulong)((v & 0x00000001UL) != 0UL);
		v >>= 1;
		v |= (ulong)(carry? 0x80000000UL: 0UL);
	}

	return v;
}
