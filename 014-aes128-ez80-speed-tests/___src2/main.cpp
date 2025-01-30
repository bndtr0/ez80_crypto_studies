#include <stdio.h>
#include <string.h>
#include "aes128.h"
#include "numtools.h"


extern char msg[1024], key[32], outmsg[1024];
extern byte aesStateMat[4][4];

int main()
{
	strcpy(msg, "The fox jumps over the fence.");
	//strcpy(key, "1234567890123456");
	
	//strcpy(msg, "\x32\x43\xf6\xa8\x88\x5a\x30\x8d\x31\x31\x98\xa2\xe0\x37\x07\x34");
	//strcpy(key, "\x2b\x7e\x15\x16\x28\xae\xd2\xa6\xab\xf7\x15\x88\x09\xcf\x4f\x3c");
	
	//strcpy(msg, "abcdefghijklmnop");
	strcpy(key, "1234567890123456");
	//strcpy(key, "0123456789012345");
	
	aes128_crypt((byte*)msg, (byte*)key, (byte*)outmsg, strlen(msg));
	
	int l = strlen(msg);
	if(l % 16 == 0)
		NumTools_printArray_b((byte*)outmsg, l, 16, true);
	else
		NumTools_printArray_b((byte*)outmsg, l + (16 - l % 16), 16, true);
	
	return 0;
}
