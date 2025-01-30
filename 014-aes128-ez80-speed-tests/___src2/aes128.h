#ifndef ___AES128_H____
#define ___AES128_H____

typedef unsigned char byte;
typedef unsigned int  uint;
typedef unsigned long ulong;


byte *aes128_crypt(byte* msgInputArr, byte* keyArr, byte *msgOutputArr, int msgLen);
void  aes128_calculate(byte* msgInputArr, byte* keyArr, byte *msgOutputArr);

void aes_key_expansion(byte *keyArr, ulong *keyScheduleArr);
void aes_sub_bytes(byte stateMat[4][4]);
void aes_shift_rows(byte stateMat[4][4]);
void aes_mix_columns(byte stateMat[4][4]);
void aes_add_round_key(byte stateMat[4][4], ulong* keySchedule, byte round);


byte aes_gf_mult(byte f1, byte f2);
ulong aes_rot_word(ulong w);
ulong aes_sub_word(ulong w);

void aes_copy_msg_bytes_to_state(byte st[4][4], byte *msgin);
void aes_copy_states_bytes_to_output(byte st[4][4], byte *msgout, int msgCurrentPos);

void printMatrix4x4(char *title, byte st[4][4]);
void printArray_b(byte *arr, int maxLength, bool withSpaces, bool withFinalNewLine);
void printArray_ul(ulong *arr, int maxLength, int cols, bool withSpaces, bool withFinalNewLine);

#endif

