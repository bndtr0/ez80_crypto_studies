#ifndef ___SHA224_H___
#define ___SHA224_H___
#include "numtools.h"

char* sha224_hash(byte *arrData, long totalDataLen);
void sha224_calculate(ulong *W);

ulong rol(ulong v, int times);
ulong ror(ulong v, int times);

#endif