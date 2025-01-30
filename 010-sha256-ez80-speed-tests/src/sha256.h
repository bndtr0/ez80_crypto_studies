#ifndef ___SHA256_H___
#define ___SHA256_H___
#include "numtools.h"

char* sha256_hash(byte *arrData, long totalDataLen);
void sha256_calculate(ulong *W);

ulong rol(ulong v, int times);
ulong ror(ulong v, int times);

#endif