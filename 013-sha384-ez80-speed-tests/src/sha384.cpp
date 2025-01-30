/****************************************************************
	2024/09/06 13:40 NZT
	SHA-384 software implementation

	(following implementation from the book "" and my previous
	  implementation adapted to Microsoft C#)
  * Translated into Agon Light (Zilog eZ80L92 MPU) SDK and data types.

*****************************************************************/
#include <string.h>
#include "sha384.h"
#include "numtools.h"

#define DEBUG_MODE 0


ulong64 h[8], aa, bb, cc, dd, ee, ff, gg, hh;
ulong64 M[16], W[80];
byte    block128[128];
ulong   paddingRemainingBytes;
boolean usePadding, paddingFirstByte, lastBlock;
char    finalHashArr[64];

ulong64 k[80] = {
	0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
	0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
	0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
	0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
	0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
	0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
	0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
	0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
	0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
	0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
	0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
	0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
	0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
	0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
	0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
	0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
	0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
	0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
	0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
	0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
	};


char* sha384_hash(byte *arrData, long totalDataLen)
{
	lastBlock = false;
	usePadding = (totalDataLen /*arrData.Length*/ > 0? false: true);      // If input data is an empty string, just add a padding for the process.
	paddingFirstByte = (totalDataLen /*arrData.Length*/ > 0? false : true); 
	paddingRemainingBytes = 0;

	// Initialise internal state for SHA-384 calculations (different from SHA-224 and SHA-256 initial constants).
	h[0] = 0xcbbb9d5dc1059ed8ULL;
	h[1] = 0x629a292a367cd507ULL;
	h[2] = 0x9159015a3070dd17ULL;
	h[3] = 0x152fecd8f70e5939ULL;
	h[4] = 0x67332667ffc00b31ULL;
	h[5] = 0x8eb44a8768581511ULL;
	h[6] = 0xdb0c2e0d64f98fa7ULL;
	h[7] = 0x47b5481dbefa4fa4ULL;

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
		for (int j = 0; j < 128; j++)
		{
			updateMicros();
		
			if (usePadding == false)
			{
				block128[j] = arrData[i++];
				if(i == totalDataLen)
				{
					usePadding = true;
					paddingFirstByte = true;
					paddingRemainingBytes = (j < 111? 111-j: 240-j-1);

					/*if(DEBUG_MODE >= 3)
					{
						printf("PaddingRemainingBytes = ");
						printf(paddingRemainingBytes);
					}*/
				}
			}
			else
			{
				if(paddingFirstByte == true)
				{
					paddingFirstByte = false;
					block128[j] = 0x80;
					paddingRemainingBytes--;
				}
				else
				{
					if (paddingRemainingBytes > 0)
					{
						block128[j] = 0x00;
						paddingRemainingBytes--;
					}
					else
					{
						/*if (DEBUG_MODE >= 3)
							Serial.println("lastblock");*/

						lastBlock = true;

						// Add total data length (before padding) to the end of this last block.
						totalDataLen *= 8;      // Length in bytes becomes length in bits.

						block128[127] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block128[126] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block128[125] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block128[124] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block128[123] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block128[122] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block128[121] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block128[120] = (byte)(totalDataLen & 0xff);
						block128[119] = 0x00;			// for a message length < 2^32
						block128[118] = 0x00;
						block128[117] = 0x00;
						block128[116] = 0x00;
						block128[115] = 0x00;
						block128[114] = 0x00;
						block128[113] = 0x00;
						block128[112] = 0x00;            
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

    // Pack 8 contiguous bytes (four 8-bits, in LSB format) into one M array cell (with 64-bits, in LSB format)
    for (int j = 0, k = 0; j < 16; j++, k += 8)         // indexes 0->13: data or padding / indexes 14->15: total data length
    {
        M[j]  = (ulong64)block128[k + 7];
        M[j] |= ((ulong64)block128[k + 6]) << 8;
        M[j] |= ((ulong64)block128[k + 5]) << 16;
        M[j] |= ((ulong64)block128[k + 4]) << 24;
        M[j] |= ((ulong64)block128[k + 3]) << 32;
        M[j] |= ((ulong64)block128[k + 2]) << 40;
        M[j] |= ((ulong64)block128[k + 1]) << 48;
        M[j] |= ((ulong64)block128[k]) << 56;

        W[j] = M[j];
    }

    // Now, transform 16 64-bits words from W[0] to W[15] into 64 64-bits words
		//   inside W[16] to W[79]
	for(int t = 16; t <= 79; t++)
	{
		W[t] = rho1(W[t-2]) + W[t-7] + rho0(W[t-15]) + W[t-16];
	}

    /*if(DEBUG_MODE >= 2)
        NumTools_printArray_ui(M, 16, 16, true, 8);*/

    sha384_calculate(W);
      
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

	strcpy(finalHashArr, NumTools_toHex16(h[0]));
	strcat(finalHashArr, NumTools_toHex16(h[1]));
	strcat(finalHashArr, NumTools_toHex16(h[2]));
	strcat(finalHashArr, NumTools_toHex16(h[3]));
	strcat(finalHashArr, NumTools_toHex16(h[4]));
	strcat(finalHashArr, NumTools_toHex16(h[5]));
	//strcat(finalHashArr, NumTools_toHex16(h[6]));		// Not used in SHA-384, but only in SHA-512.
	//strcat(finalHashArr, NumTools_toHex16(h[7]));		// Not used in SHA-384, but only in SHA-512.

	return finalHashArr;
}


void sha384_calculate(ulong64 *W)
{
	aa = h[0];      // 8 accumulators of 64-bits length.
	bb = h[1];
	cc = h[2];
	dd = h[3];
	ee = h[4];
	ff = h[5];
	gg = h[6];
	hh = h[7];

	ulong64 temp1, temp2;
	for(byte t = 0; t <= 79; t++)
	{
		temp1 = hh + sigma1(ee) + Ch(ee, ff, gg) + k[t] + W[t];
		temp2 = sigma0(aa) + Maj(aa, bb, cc);

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
// Implements ROL (rotate-on-left) bit function over 64-bits unsigned data.
ulong64 rol(ulong64 v, int times)
{
	boolean carry = false;

	for(int i = 1; i <= times; i++)
	{
		carry = (ulong64)((v & 0x8000000000000000ULL) != 0ULL);
		v <<= 1;
		v |= (ulong64)(carry? 1ULL: 0ULL);
	}

	return v;
}


//---------------------------------------------------------------
// Implements ROR (rotate-on-right) bit function over 64-bits unsigned data.
ulong64 ror(ulong64 v, int times)
{
	boolean carry = false;

	for(int i = 1; i <= times; i++)
	{
		carry = (ulong64)((v & 0x00000001ULL) != 0ULL);
		v >>= 1;
		v |= (ulong64)(carry? 0x8000000000000000ULL: 0ULL);
	}

	return v;
}

//---------------------------------------------------------------
// Implements Ch bit function over 64-bits unsigned data.
ulong64 Ch(ulong64 x, ulong64 y, ulong64 z)
{
	return (x & y) ^ (~x & z);
}

//---------------------------------------------------------------
// Implements Maj bit function over 64-bits unsigned data.
ulong64 Maj(ulong64 x, ulong64 y, ulong64 z)
{
	return (x & y) ^ (x & z) ^ (y & z);
}


//---------------------------------------------------------------
// Implements sigma0 bit function over 64-bits unsigned data.
ulong64 sigma0(ulong64 v)
{
	return ror(v, 28) ^ ror(v, 34) ^ ror(v, 39);
}


//---------------------------------------------------------------
// Implements sigma1 bit function over 64-bits unsigned data.
ulong64 sigma1(ulong64 v)
{
	return ror(v, 14) ^ ror(v, 18) ^ ror(v, 41);
}


//---------------------------------------------------------------
// Implements rho0 bit function over 64-bits unsigned data.
ulong64 rho0(ulong64 v)
{
	return ror(v, 1) ^ ror(v, 8) ^ (v >> 7);
}


//---------------------------------------------------------------
// Implements rho1 bit function over 64-bits unsigned data.
ulong64 rho1(ulong64 v)
{
	return ror(v, 19) ^ ror(v, 61) ^ (v >> 6);
}
