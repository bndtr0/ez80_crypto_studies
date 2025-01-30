#ifndef ___SHA384_H___
#define ___SHA384_H___
#include "numtools.h"

char* sha384_hash(byte *arrData, long totalDataLen);
void sha384_calculate(ulong64 *W);

ulong64 rol(ulong64 v, int times);
ulong64 ror(ulong64 v, int times);
ulong64 Ch(ulong64 x, ulong64 y, ulong64 z);
ulong64 Maj(ulong64 x, ulong64 y, ulong64 z);
ulong64 sigma0(ulong64 v);
ulong64 sigma1(ulong64 v);
ulong64 rho0(ulong64 v);
ulong64 rho1(ulong64 v);
#endif
