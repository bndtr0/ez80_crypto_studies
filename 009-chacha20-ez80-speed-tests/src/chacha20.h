#ifndef ___CHACHA20_H___
#define ___CHACHA20_H___
#include "numtools.h"

byte* chacha20_crypt(char *msg, char *key, byte *nonce, uint counter, byte* outputBuffer,
				 uint msgLen, uint keyLen, boolean useThisCounter);
byte* chacha20_calculate(byte *msgArr, byte* keyArr, byte* nonceArr, uint counter, byte* outputBuffer, int msgLen);

uint rol(uint v, uint times);
uint LSB(byte a, byte b, byte c, byte d);
uint LSB_s4(char *s);
void QuarterRound(uint *M, int ia, int ib, int ic, int id);
void arrayClear(uint *arrd);
void arrayCopy(uint *arrd, uint *arrs);
void arraySum(uint *arrd, uint *arrs);

#endif
