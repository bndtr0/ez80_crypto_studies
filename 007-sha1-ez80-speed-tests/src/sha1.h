#ifndef ___SHA1_H___
#define ___SHA1_H___
#include "numtools.h"

char* sha1_hash(byte *arrData, long totalDataLen);
void sha1_calculate(ulong *W);

ulong rol(ulong v, int times);
ulong Ft(ulong x, ulong y, ulong z, byte t);
ulong Kt(byte t);

#endif