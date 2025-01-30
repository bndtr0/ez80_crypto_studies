#ifndef ___NUMTOOLS_H___
#define ___NUMTOOLS_H___
#include <stdio.h>
#include <stdint.h>

typedef int boolean;
typedef uint8_t byte;
typedef unsigned short ushort;
typedef uint32_t uint;
typedef uint64_t ulong;

#define true 1
#define false 0
#define TRUE 1
#define FALSE 0
#define True 1
#define False 0


void  micros_setup();
void  micros_stop();
uint  micros();
void  updateMicros();

char NumTools_toHexDigit(byte c);
char* NumTools_toHex(ulong d, byte maxNumOfHexDigits);
char* NumTools_toHex2(byte d);
char* NumTools_toHex4(uint d);
char* NumTools_toHex8(ulong d);
char* NumTools_toHex8Reverse(ulong d);
char* NumTools_toHex8Reverse_2b(ulong d);

void NumTools_printArray_tui(char *title, ulong* arr, uint maxElements, byte cols, bool asHex, byte hexDigits);

void NumTools_printArray_b(byte *arr, int maxElements, byte cols, bool asHex, bool inoneline, bool withSpaces);
void NumTools_printArray_bf(FILE *f, byte *arr, int maxElements, byte cols, bool asHex, bool inoneline, bool withSpaces);
void NumTools_printArray_ui(uint *arr, int maxElements, byte cols, bool asHex, byte hexDigits);
void NumTools_printArray_ul(ulong *arr, int maxElements, byte cols, bool asHex, byte hexDigits);

#endif
