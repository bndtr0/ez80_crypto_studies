/********************************************************

	2024/09/11 16:00 NZT
	AES-256 implementation
	
	* Based on FIPS-197 document (FIPS.197-upd1.pdf).
 	* Translated into Agon Light 2 (Zilog eZ80F92) environment and data types

*********************************************************/
#include <stdio.h>
#include <string.h>
#include "aes256.h"
#include "numtools.h"

#define ENABLE_AES_256 1

#ifdef ENABLE_AES_128
#define Nb 4		// Number of 32-bits words (number of columns)
#define Nk 4		// key length (in 32-bits words) -> 4 words of 32-bits -> 16 bytes -> 128 bits
#define Nr 10		// number of rounds
#endif

#ifdef ENABLE_AES_192
#define Nb 4		// Number of 32-bits words (number of columns)
#define Nk 6		// key length (in 32-bits words) -> 6 words of 32-bits -> 24 bytes -> 192 bits
#define Nr 12		// number of rounds
#endif

#ifdef ENABLE_AES_256
#define Nb 4		// Number of 32-bits words (number of columns)
#define Nk 8		// key length (in 32-bits words) -> 8 words of 32-bits -> 32 bytes -> 256 bits
#define Nr 14		// number of rounds
#endif


byte SBOX[256] = {
//  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
};
  
  
byte rsbox[256] = {
//  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d 
}; 


byte Rcon[11] = { 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

byte aesStateMat[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
ulong w[4 * (Nr + 1)];


//-----------------------------------------------------------------------------------
byte *aes256_crypt(byte* msgInputArr, byte* keyArr, byte *msgOutputArr, int msgLen)
{
	int ml = msgLen & 0xfffffff0;
	int i = 0;
	
	// Process every 16-bytes block from original message until last block with less than 16-bytes.
	for(i = 0; i < ml; i += 16)
	{
		aes256_calculate(msgInputArr + i, keyArr, msgOutputArr + i);
		//Serial.print("[[1]] ");
		//NumTools_printArray_b((byte*)msgOutputArr, (strlen((char*)msgInputArr) >> 4) << 4, 16, true);
	}
	
	// Process last block, adding a padding of zeros until 16-bytes.
	if(ml <= msgLen)
	{
		//printf("[last block]\n");
		for(int j = msgLen, i = 16 - (msgLen & 0x0f); i > 0; i--, j++)
			msgInputArr[j] = '\0';
		
		aes256_calculate(msgInputArr + ml, keyArr, msgOutputArr + ml);
		//Serial.print("[[2]] ");
		//NumTools_printArray_b((byte*)msgOutputArr, (strlen((char*)msgInputArr)), 16, true);
	}
	
	return msgOutputArr;
}

//-----------------------------------------------------------------------------------
void aes256_calculate(byte* msgInputArr, byte* keyArr, byte *msgOutputArr)
{	
	// 0. Copy 16 bytes from original message to the State Array.
	aes_copy_msg_bytes_to_state(aesStateMat, (byte*)msgInputArr);

	// 1. AES (initial) key expansion
	aes_key_expansion((byte*)keyArr, w);
	
	/*printMatrix4x4("(after key expansion 0):", aesStateMat);
	printf("\nw[0..43]:\n");
	printArray_ul(w, 43, 6, true, true);	*/
	
	// 2. Initial round key addition (AddRoundKey)
		// 2.1 AddRoundKey
	aes_add_round_key(aesStateMat, w, 0);
	
	//printMatrix4x4("(after add round key 0):", aesStateMat);

	// 3. Do 9 rounds.
	for(int roundNo = 1; roundNo <= Nr - 1; roundNo++)
	{
		// 3.1 SubBytes
		aes_sub_bytes(aesStateMat);
		//printMatrix4x4("(after sub bytes):", aesStateMat);
		
		// 3.2 ShiftRows
		aes_shift_rows(aesStateMat);
		//printMatrix4x4("(after shift rows):", aesStateMat);
		
		// 3.3 MixColumns
		aes_mix_columns(aesStateMat);
		//printMatrix4x4("(after mix columns):", aesStateMat);
		
		// 3.4 AddRoundKey
		aes_add_round_key(aesStateMat, w, roundNo);
		//printMatrix4x4("(after add round key):", aesStateMat);
		
		//printf("---------------------------------------\n");	
	}
	
	// 4. Last round
		// 4.1 SubBytes
	aes_sub_bytes(aesStateMat);
		
		// 4.2 ShiftRows
	aes_shift_rows(aesStateMat);
		
		// 4.3 AddRoundKey
	aes_add_round_key(aesStateMat, w, Nr);
	
	// 5. Return state (and add to encrypted output byte array).
	byte *p = msgOutputArr;
	for(int col = 0; col <= 3; col++)
		for(int row = 0; row <= 3; row++)
		{
			*p++ = aesStateMat[row][col];
		}
			
	//printByteArray((byte*)aesStateMat, 16, true, true);
	//printMatrix4x4("AES state matrix:", aesStateMat);
	
	//return msgOutputArr;
}



//--------------------------------------------------------------------

/*--------------------------------------------------------------------
	KeyExpansion that generates 4*(Nr + 1) 32-bits words from the key data array.
	  Returns a linear array of 32-bits words (w[i]) in the range 
	  0 <= i <= 4*(Nr + 1) 
----------------------------------------------------------------------*/
void aes_key_expansion(byte *keyArr, ulong *keyScheduleArr)
{
	for(byte i = 0, ki = 0; i <= Nk - 1; i++, ki += 4)
	{
		ulong b;
		b = keyArr[ki + 0];
		b = (b << 8) | keyArr[ki + 1];
		b = (b << 8) | keyArr[ki + 2];
		b = (b << 8) | keyArr[ki + 3];
		
		keyScheduleArr[i] = b;	
	}	
	
	for(byte i = Nk; i <= 4 * Nr + 3; i++)
	{
		ulong temp = keyScheduleArr[i - 1];							//printf("(temp[%d] = %8.8x\n", i, temp);
		if(i % Nk == 0)
		{
			//printf("(rot_word[%d] => %8.8x => %8.8x\n", i, temp, aes_rot_word(temp));
			//printf("(aes_sub_word[%d] = %8.8x\n", i, aes_sub_word(aes_rot_word(temp)));
			
			temp = aes_sub_word(aes_rot_word(temp)) ^ ((ulong)Rcon[i / Nk] << 24);
		}
		else if(Nk > 6 && (i % Nk == 4))
			temp = aes_sub_word(temp);
		
		//printf("(temp[%d] = %8.8x\n", i, temp);
		
		keyScheduleArr[i] = keyScheduleArr[i - Nk] ^ temp;
		//printf("(w[%d] = %8.8x\n\n", i, keyScheduleArr[i]);		
	}
}


/*--------------------------------------------------------------------
	SubBytes do some substitutions inside the state matrix, using
	  the SBOX() matrix (with precalculated values)
----------------------------------------------------------------------*/
void aes_sub_bytes(byte stateMat[4][4])
{
	for(int r = 0; r < 4; r++)
		for(int c = 0; c < 4; c++)
		{
			stateMat[r][c] = SBOX[stateMat[r][c]];	
		}
}


/*--------------------------------------------------------------------
	ShiftRows do 
----------------------------------------------------------------------*/
void aes_shift_rows(byte stateMat[4][4])
{
	byte aux1, aux2;
	
	// Shift row 1 to the left 1 position
	aux1 = stateMat[1][0];
	stateMat[1][0] = stateMat[1][1];	
	stateMat[1][1] = stateMat[1][2];
	stateMat[1][2] = stateMat[1][3];
	stateMat[1][3] = aux1;
	
	// Shift row 2 to the left 2 positions
	aux1 = stateMat[2][0];
	aux2 = stateMat[2][1];
	stateMat[2][0] = stateMat[2][2];	
	stateMat[2][1] = stateMat[2][3];
	stateMat[2][2] = aux1;
	stateMat[2][3] = aux2;
	
	// Shift row 3 to the left 3 positions
	aux1 = stateMat[3][3];
	stateMat[3][3] = stateMat[3][2];	
	stateMat[3][2] = stateMat[3][1];
	stateMat[3][1] = stateMat[3][0];
	stateMat[3][0] = aux1;	
}


/*--------------------------------------------------------------------
	MixColumns do 
----------------------------------------------------------------------*/
void aes_mix_columns(byte stateMat[4][4])
{
	byte s0, s1, s2, s3;
	
	for(int c = 0; c < 4; c++)
	{
		s0 = aes_gf_mult(0x02, stateMat[0][c]) ^ aes_gf_mult(0x03, stateMat[1][c]) ^ stateMat[2][c] ^ stateMat[3][c];
		s1 = stateMat[0][c] ^ aes_gf_mult(0x02, stateMat[1][c]) ^ aes_gf_mult(0x03, stateMat[2][c]) ^ stateMat[3][c];
		s2 = stateMat[0][c] ^ stateMat[1][c] ^ aes_gf_mult(0x02, stateMat[2][c]) ^ aes_gf_mult(0x03, stateMat[3][c]);
		s3 = aes_gf_mult(0x03, stateMat[0][c]) ^ stateMat[1][c] ^ stateMat[2][c] ^ aes_gf_mult(0x02, stateMat[3][c]);
		
		stateMat[0][c] = s0;
		stateMat[1][c] = s1;
		stateMat[2][c] = s2;
		stateMat[3][c] = s3;
	}
}


/*--------------------------------------------------------------------
	Special multiplication over GF(2^8) (see page 8 & 9) 
----------------------------------------------------------------------*/
byte aes_gf_mult(byte f1, byte f2)
{
	switch(f1)
	{
		case 0x00:  return 0x00;		// unused by AES
		case 0x01:  return f2;
		case 0x02:  return (f2 <= 0x7f? f2 << 1: (f2 << 1) ^ 0x1b);
		case 0x03:  return (f2 ^ (f2 <= 0x7f? f2 << 1: (f2 << 1) ^ 0x1b));
		default: return 0x00;
	}
}


/*--------------------------------------------------------------------
	RotWord operation over an unsigned long 32-bits word (see page 17)
----------------------------------------------------------------------*/
ulong aes_rot_word(ulong w)
{
	ulong lout;
	
	//printf("---w    = %8.8x\n", w);
	lout  = ((w & 0x00ffffff) << 8);
	//printf("---lout = %8.8x\n", lout);
	w >>= 24;
	w &= 0xff;
	//printf("---w    = %8.8x\n", w);
	lout |= w;
	//printf("---lout = %8.8x\n", lout);
	
	return lout;
}


/*--------------------------------------------------------------------
	SubWord operation over an unsigned long 32-bits word (see page 17).
	  Uses the SBOX data to generate and return a new 32-bits word.
----------------------------------------------------------------------*/
ulong aes_sub_word(ulong w)
{
	ulong lout;
	
	lout = SBOX[w & 0xff];
	w >>= 8;
	lout |= ((ulong)SBOX[w & 0xff] << 8);
	w >>= 8;
	lout |= ((ulong)SBOX[w & 0xff] << 16);
	w >>= 8;
	lout |= ((ulong)SBOX[w & 0xff] << 24);

	return lout;
}




/*--------------------------------------------------------------------
	AddRoundKey uses the previous Key Expansion data to transform
	  the state in AES. 
----------------------------------------------------------------------*/
void aes_add_round_key(byte stateMat[4][4], ulong* keySchedule, byte roundNo)
{
	for(byte c = 0; c <= 3; c++)
	{
		ulong temp = keySchedule[4 * roundNo + c];
		
		stateMat[3][c] ^= temp & 0xff;
		temp >>= 8;
		stateMat[2][c] ^= temp & 0xff;
		temp >>= 8;
		stateMat[1][c] ^= temp & 0xff;
		temp >>= 8;
		stateMat[0][c] ^= temp & 0xff;
	}
}






//--------------------------------------------------------------------
void aes_inv_mix_columns()
{
	
}
















//-----------------------------------------------------------------------------------
void aes_copy_msg_bytes_to_state(byte st[4][4], byte *msgin)
{
	for(int r = 0; r < 4; r++)
		for(int c = 0; c < 4; c++)
			st[r][c] = msgin[r + 4*c];
}


//-----------------------------------------------------------------------------------
void aes_copy_states_bytes_to_output(byte st[4][4], byte *msgout, int msgCurrentPos)
{
	for(int r = 0; r < 4; r++)
		for(int c = 0; c < 4; c++)
			msgout[r + 4*c + msgCurrentPos] = st[r][c];
}


//-----------------------------------------------------------------------------------
void printMatrix4x4(char *title, byte st[4][4])
{
	printf("%s\n", title);
	
	for(int r = 0; r < 4; r++)
	{
		for(int c = 0; c < 4; c++)
		{
			printf("%2.2x ", st[r][c]);
		}
		
		printf("\n");
	}
	
	printf("\n");
}


//-----------------------------------------------------------------------------------
void printArray_b(byte *arr, int maxLength, boolean withSpaces, boolean withFinalNewLine)
{
	for(int i = 0; i < maxLength; i++)
	{
		printf("%2.2x", arr[i]);
	
		if(withSpaces)
			printf(" ");
	}
	
	if(withFinalNewLine)
		printf("\n");
}


//-----------------------------------------------------------------------------------
void printArray_ul(ulong *arr, int maxLength, int cols, boolean withSpaces, boolean withFinalNewLine)
{
	for(int i = 0; i < maxLength; i++)
	{
		printf("%8.8lx", arr[i]);
	
		if(withSpaces)
			printf(" ");
			
		if((i + 1) % cols == 0)
			printf("\n");
	}
	
	if(withFinalNewLine)
		printf("\n");
}
