/****************************************************************
	2024/08/20 14:20 NZT
	MD5 hash software implementation

	* (following implementation from the book "" and my previous
	    implementation adapted to Microsoft C#)
 	* Translated into Zilog eZ80 CEdev/AgDev C/C++ environment and data types
 	    for Agon Light 2.
*****************************************************************/
#include <string.h>
#include "md5.h"
#include "numtools.h"

#define DEBUG_MODE 0


ulong   A, B, C, D, aa, bb, cc, dd;
ulong   paddingRemainingBytes;
bool    usePadding, paddingFirstByte, lastBlock;
char    finalHashArr[40];


char* md5_hash(byte *arrData, long totalDataLen)
{
	ulong M[16];
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

/*
	if(DEBUG_MODE >= 3)
  {
		Serial.print("A=");
    Serial.print(A, HEX);
    Serial.print("  B=");
    Serial.print(B, HEX);
    Serial.print("  C=");
    Serial.print(C, HEX);
    Serial.print("  D=");
    Serial.println(D, HEX);
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

						block64[56] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[57] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[58] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[59] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[60] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[61] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[62] = (byte)(totalDataLen & 0xff);
						totalDataLen >>= 8;
						block64[63] = (byte)(totalDataLen & 0xff);

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
        M[j]  = (ulong)block64[k];
        M[j] |= ((ulong)block64[k + 1]) << 8;
        M[j] |= ((ulong)block64[k + 2]) << 16;
        M[j] |= ((ulong)block64[k + 3]) << 24;
    }

    /*if(DEBUG_MODE >= 2)
        NumTools_printArray_ui(M, 16, 16, true, 8);
	*/

    md5_calculate(M);
      
    /*if(DEBUG_MODE >= 3)
    {
      Serial.print("A=");
      Serial.print(A, HEX);
      Serial.print("  B=");
      Serial.print(B, HEX);
      Serial.print("  C=");
      Serial.print(C, HEX);
      Serial.print("  D=");
      Serial.println(D, HEX);
      Serial.println();
    }*/
    //    Serial.println("A=%8.8x  B=%8.8x  C=%8.8x  D=%8.8x\r\r", A, B, C, D);*/
  }

  strcpy(finalHashArr, NumTools_toHex8Reverse_2b(A));
  strcat(finalHashArr, NumTools_toHex8Reverse_2b(B));
  strcat(finalHashArr, NumTools_toHex8Reverse_2b(C));
  strcat(finalHashArr, NumTools_toHex8Reverse_2b(D));

  return finalHashArr;
}


void md5_calculate(ulong *M)
{
	aa = A;                 // 4 accumulators of 32-bits length.
	bb = B;
	cc = C;
	dd = D;

	// Round 1
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	/*	Serial.print("aa=");
    Serial.print(aa, HEX);
    Serial.print("  bb=");
    Serial.print(bb, HEX);
    Serial.print("  cc=");
    Serial.print(cc, HEX);
    Serial.print("  dd=");
    Serial.println(dd, HEX);*/  
	FF(&aa, bb, cc, dd, M[0], 7, 0xd76aa478UL);
	FF(&dd, aa, bb, cc, M[1], 12, 0xe8c7b756UL);
	FF(&cc, dd, aa, bb, M[2], 17, 0x242070dbUL);
	FF(&bb, cc, dd, aa, M[3], 22, 0xc1bdceeeUL);
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	FF(&aa, bb, cc, dd, M[4], 7, 0xf57c0fafUL);
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	FF(&dd, aa, bb, cc, M[5], 12, 0x4787c62aUL);
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	FF(&cc, dd, aa, bb, M[6], 17, 0xa8304613UL);
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	FF(&bb, cc, dd, aa, M[7], 22, 0xfd469501UL);
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	FF(&aa, bb, cc, dd, M[8], 7, 0x698098d8UL);
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	FF(&dd, aa, bb, cc, M[9], 12, 0x8b44f7afUL);
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	FF(&cc, dd, aa, bb, M[10], 17, 0xffff5bb1UL);
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	FF(&bb, cc, dd, aa, M[11], 22, 0x895cd7beUL);
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	FF(&aa, bb, cc, dd, M[12], 7, 0x6b901122UL);
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	FF(&dd, aa, bb, cc, M[13], 12, 0xfd987193UL);
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	FF(&cc, dd, aa, bb, M[14], 17, 0xa679438eUL);
	//Console.WriteLine("aa=" + NumTools.toHex8(aa) + " bb=" + NumTools.toHex8(bb) + " cc=" + NumTools.toHex8(cc) + " dd=" + NumTools.toHex8(dd));
	FF(&bb, cc, dd, aa, M[15], 22, 0x49b40821UL);

	/*if (DEBUG_MODE >= 4) 
	{
		Serial.print("FF(): aa=");
		Serial.print(aa, HEX);
		Serial.print("  bb=");
		Serial.print(bb, HEX);
		Serial.print("  cc=");
		Serial.print(cc, HEX);
		Serial.print("  dd=");
		Serial.println(dd, HEX);
		Serial.println();
	}*/

	// Round 2
	GG(&aa, bb, cc, dd, M[1], 5, 0xf61e2562UL);
	GG(&dd, aa, bb, cc, M[6], 9, 0xc040b340UL);
	GG(&cc, dd, aa, bb, M[11], 14, 0x265e5a51UL);
	GG(&bb, cc, dd, aa, M[0], 20, 0xe9b6c7aaUL);
	GG(&aa, bb, cc, dd, M[5], 5, 0xd62f105dUL);
	GG(&dd, aa, bb, cc, M[10], 9, 0x02441453UL);
	GG(&cc, dd, aa, bb, M[15], 14, 0xd8a1e681UL);
	GG(&bb, cc, dd, aa, M[4], 20, 0xe7d3fbc8UL);
	GG(&aa, bb, cc, dd, M[9], 5, 0x21e1cde6UL);
	GG(&dd, aa, bb, cc, M[14], 9, 0xc33707d6UL);
	GG(&cc, dd, aa, bb, M[3], 14, 0xf4d50d87UL);
	GG(&bb, cc, dd, aa, M[8], 20, 0x455a14edUL);  // error -> cd instead of ed (corrected on 2024-07-21)
	GG(&aa, bb, cc, dd, M[13], 5, 0xa9e3e905UL);
	GG(&dd, aa, bb, cc, M[2], 9, 0xfcefa3f8UL);
	GG(&cc, dd, aa, bb, M[7], 14, 0x676f02d9UL);
	GG(&bb, cc, dd, aa, M[12], 20, 0x8d2a4c8aUL);

/*	if (DEBUG_MODE >= 4)
  {
		Serial.print("GG(): aa=");
    Serial.print(aa, HEX);
    Serial.print("  bb=");
    Serial.print(bb, HEX);
    Serial.print("  cc=");
    Serial.print(cc, HEX);
    Serial.print("  dd=");
    Serial.println(dd, HEX);
  }*/

	// Round 3
	HH(&aa, bb, cc, dd, M[5], 4, 0xfffa3942UL);
	HH(&dd, aa, bb, cc, M[8], 11, 0x8771f681UL);
	HH(&cc, dd, aa, bb, M[11], 16, 0x6d9d6122UL);
	HH(&bb, cc, dd, aa, M[14], 23, 0xfde5380cUL);
	HH(&aa, bb, cc, dd, M[1], 4, 0xa4beea44UL);
	HH(&dd, aa, bb, cc, M[4], 11, 0x4bdecfa9UL);
	HH(&cc, dd, aa, bb, M[7], 16, 0xf6bb4b60UL);
	HH(&bb, cc, dd, aa, M[10], 23, 0xbebfbc70UL);
	HH(&aa, bb, cc, dd, M[13], 4, 0x289b7ec6UL);
	HH(&dd, aa, bb, cc, M[0], 11, 0xeaa127faUL);
	HH(&cc, dd, aa, bb, M[3], 16, 0xd4ef3085UL);
	HH(&bb, cc, dd, aa, M[6], 23, 0x04881d05UL);
	HH(&aa, bb, cc, dd, M[9], 4, 0xd9d4d039UL);
	HH(&dd, aa, bb, cc, M[12], 11, 0xe6db99e5UL);  // error -> c5 instead of e5 (corrected on 2024-07-21)
	HH(&cc, dd, aa, bb, M[15], 16, 0x1fa27cf8UL);
	HH(&bb, cc, dd, aa, M[2], 23, 0xc4ac5665UL);

/*	if (DEBUG_MODE >= 4)
  {
		Serial.print("HH(): aa=");
    Serial.print(aa, HEX);
    Serial.print("  bb=");
    Serial.print(bb, HEX);
    Serial.print("  cc=");
    Serial.print(cc, HEX);
    Serial.print("  dd=");
    Serial.println(dd, HEX);
  }*/

	// Round 4
	II(&aa, bb, cc, dd, M[0], 6, 0xf4292244UL);
	II(&dd, aa, bb, cc, M[7], 10, 0x432aff97UL);
	II(&cc, dd, aa, bb, M[14], 15, 0xab9423a7UL);
	II(&bb, cc, dd, aa, M[5], 21, 0xfc93a039UL);
	II(&aa, bb, cc, dd, M[12], 6, 0x655b59c3UL);
	II(&dd, aa, bb, cc, M[3], 10, 0x8f0ccc92UL);
	II(&cc, dd, aa, bb, M[10], 15, 0xffeff47dUL);
	II(&bb, cc, dd, aa, M[1], 21, 0x85845dd1UL);
	II(&aa, bb, cc, dd, M[8], 6, 0x6fa87e4fUL);
	II(&dd, aa, bb, cc, M[15], 10, 0xfe2ce6e0UL);
	II(&cc, dd, aa, bb, M[6], 15, 0xa3014314UL);
	II(&bb, cc, dd, aa, M[13], 21, 0x4e0811a1UL);
	II(&aa, bb, cc, dd, M[4], 6, 0xf7537e82UL);
	II(&dd, aa, bb, cc, M[11], 10, 0xbd3af235UL);
	II(&cc, dd, aa, bb, M[2], 15, 0x2ad7d2bbUL);
	II(&bb, cc, dd, aa, M[9], 21, 0xeb86d391UL);

/*	if (DEBUG_MODE >= 4)
  {
		Serial.print("II(): aa=");
    Serial.print(aa, HEX);
    Serial.print("  bb=");
    Serial.print(bb, HEX);
    Serial.print("  cc=");
    Serial.print(cc, HEX);
    Serial.print("  dd=");
    Serial.println(dd, HEX);
  }*/

	// Final sum and accumulation.
	A += aa;
	B += bb;
	C += cc;
	D += dd;

/*	if (DEBUG_MODE >= 4)
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
ulong F_(ulong x, ulong y, ulong z)
{
	return (x & y) | ((~x) & z);
}

ulong G_(ulong x, ulong y, ulong z)
{
	return (x & z) | (y & (~z));
}

ulong H_(ulong x, ulong y, ulong z)
{
	return x ^ y ^ z;
}

ulong I_(ulong x, ulong y, ulong z)
{
	return y ^ (x | (~z));
}


//---------------------------------------------------------------
void FF(ulong *ap, ulong bp, ulong cp, ulong dp, ulong Mj, int sp, ulong tip)
{
	/*if (DEBUG_MODE >= 5)
	{
		Serial.print(F(" F()  = "));
    Serial.println(NumTools_toHex8(F_(bp, cp, dp)));
		Serial.print(F(" Mj   = "));
    Serial.println(NumTools_toHex8(Mj));
		Serial.print(F(" ti   = "));
    Serial.println(NumTools_toHex8(tip));
		Serial.print(F(" sum  = "));
    Serial.println(NumTools_toHex8((*ap + F_(bp, cp, dp) + Mj + tip)));
		Serial.print(F(" ROL()= "));
    Serial.println(NumTools_toHex8(rol(*ap + F_(bp, cp, dp) + Mj + tip, sp)));
		Serial.print(F(" a    = "));
    Serial.println(NumTools_toHex8((bp + rol(*ap + F_(bp, cp, dp) + Mj + tip, sp))));
    Serial.println(F("---------------"));
	}*/

	*ap = bp + rol(*ap + F_(bp, cp, dp) + Mj + tip, sp);
}


void GG(ulong *ap, ulong bp, ulong cp, ulong dp, ulong Mj, int sp, ulong tip)
{
	*ap = bp + rol(*ap + G_(bp, cp, dp) + Mj + tip, sp);
}


void HH(ulong *ap, ulong bp, ulong cp, ulong dp, ulong Mj, int sp, ulong tip)
{
	*ap = bp + rol(*ap + H_(bp, cp, dp) + Mj + tip, sp);
}


void II(ulong *ap, ulong bp, ulong cp, ulong dp, ulong Mj, int sp, ulong tip)
{
	*ap = bp + rol(*ap + I_(bp, cp, dp) + Mj + tip, sp);
}