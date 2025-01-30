#ifndef ___NUMTOOLS_H___
#define ___NUMTOOLS_H___

typedef int boolean;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ulong64;

#define true 1
#define false 0
#define TRUE 1
#define FALSE 0
#define True 1
#define False 0

void micros_setup();
void micros_stop();
uint micros();
void updateMicros();

char NumTools_toHexDigit(byte c);
char* NumTools_toHex(ulong d, byte maxNumOfHexDigits);
char* NumTools_toHex64(ulong64 d, byte maxNumOfHexDigits);
char* NumTools_toHex2(byte d);
char* NumTools_toHex4(uint d);
char* NumTools_toHex8(ulong d);
char* NumTools_toHex16(ulong64 d);
char* NumTools_toHex8Reverse(ulong d);
char* NumTools_toHex8Reverse_2b(ulong d);

void NumTools_printArray_tui(char *title, ulong* arr, uint maxElements, byte cols, boolean asHex, byte hexDigits);

void NumTools_printArray_b(byte *arr, int maxElements, byte cols, boolean asHex);
void NumTools_printArray_ui(ulong *arr, int maxElements, byte cols, boolean asHex, byte hexDigits);

#endif
