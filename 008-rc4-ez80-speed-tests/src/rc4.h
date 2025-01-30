#ifndef ___RC4_H___
#define ___RC4_H___
#include "numtools.h"

byte* rc4_crypt(byte *msgArr, byte* keyArr, byte* outputBuffer, int msgLen, int keyLength);

#endif