#ifndef ___MD5_H___
#define ___MD5_H___
#include "numtools.h"

char* md5_hash(byte *arrData, long totalDataLen);
void md5_calculate(ulong *M);

ulong rol(ulong v, int times);
ulong F_(ulong x, ulong y, ulong z);
ulong G_(ulong x, ulong y, ulong z);
ulong H_(ulong x, ulong y, ulong z);
ulong I_(ulong x, ulong y, ulong z);

void FF(ulong *ap, ulong bp, ulong cp, ulong dp, ulong Mj, int sp, ulong tip);
void GG(ulong *ap, ulong bp, ulong cp, ulong dp, ulong Mj, int sp, ulong tip);
void HH(ulong *ap, ulong bp, ulong cp, ulong dp, ulong Mj, int sp, ulong tip);
void II(ulong *ap, ulong bp, ulong cp, ulong dp, ulong Mj, int sp, ulong tip);

#endif