/*******************************************************************************
	[2024/09/11 15:45 NZT]
	014-aes192-ez80-speed-tests
	(using eZ80 CEdev/AgDev C/C++ environment in GNU/Linux for Agon Light 2 microcomputer)
	
	* AES-192-implemented-in-software version.
	Zilog eZ80 calculated different AES-192 encryptions over different given strings
	and displays the original text with it AES-192 hash in /dev/ttyACM0 device
	(serial-to-USB device into a GNU/Linux terminal).
	1000 tests.
********************************************************************************/
#include <stdio.h>
#include <string.h>
#include "aes192.h"
#include "numtools.h"

char msg[1024], key[33], outmsg[1024];

int main() 
{
	ulong time0, time1;
	ulong delta0;

	FILE *f = fopen("log1.txt", "wt");


	printf(" Agon Light 2 (Zilog eZ80F92 Acclaim! CPU)\n");
	printf("[2024/09/11 15:45 NZT]\n");
	printf("[  AES-192 encryption test calculation over different strings  ]\n");
	printf("[     -- measure of time used for calculation of AES-192 --    ]\n");
	printf("[              -- over different string lengths  --            ]\n");
	printf("================================================================\n\n");

	fprintf(f, " Agon Light 2 (Zilog eZ80F92 Acclaim! CPU)\n");
	fprintf(f, "[2024/09/11 15:45 NZT]\n");
	fprintf(f, "[  AES-192 encryption test calculation over different strings  ]\n");
	fprintf(f, "[     -- measure of time used for calculation of AES-192 --    ]\n");
	fprintf(f, "[              -- over different string lengths  --            ]\n");
	fprintf(f, "================================================================\n\n");


	printf("Test 1\r");
	printf("======\r\r");
	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy(key, "123456789012345678901234");
		strcpy(msg, "The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.\0");
		msg[notest] = '\0';

		uint ll = strlen(msg);
		
		micros_setup();
		micros();
		time0 = micros();
		aes192_crypt((byte*)msg, (byte*)key, (byte*)outmsg, ll);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]|", notest, time0, time1, delta0);
		NumTools_printArray_b2((byte*)outmsg, ll / 16 + 1, 16, true);
		printf("|%s\n", msg);

		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]|", notest, time0, time1, delta0);
		NumTools_printArray_b2f(f, (byte*)outmsg, ll / 16 + 1, 16, true);
		fprintf(f, "|%s\n", msg);
	}



	printf("\r");
	printf("Test 2\r");
	printf("======\r\r");	
	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy(key, "lorem_ipsum_2024_09_25_P");
		strcpy(msg, "abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvw\0");
		msg[notest] = '\0';

		uint ll = strlen(msg);

		micros_setup();
		micros();
		time0 = micros();
		aes192_crypt((byte*)msg, (byte*)key, (byte*)outmsg, ll);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]|", notest, time0, time1, delta0);
		NumTools_printArray_b2((byte*)outmsg, ll / 16 + 1, 16, true);
		printf("|%s\n", msg);

		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]|", notest, time0, time1, delta0);
		NumTools_printArray_b2f(f, (byte*)outmsg, ll / 16 + 1, 16, true);
		fprintf(f, "|%s\n", msg);
	}


	printf("\r");
	printf("Test 3\r");
	printf("======\r\r");	
	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy(key, "2024_08_16_14-47_PM_NZT-");
		strcpy(msg, "0123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedc\0");
		msg[notest] = '\0';

		uint ll = strlen(msg);

		micros_setup();
		micros();
		time0 = micros();
		aes192_crypt((byte*)msg, (byte*)key, (byte*)outmsg, ll);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]|", notest, time0, time1, delta0);
		NumTools_printArray_b2((byte*)outmsg, ll / 16 + 1, 16, true);
		printf("|%s\n", msg);

		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]|", notest, time0, time1, delta0);
		NumTools_printArray_b2f(f, (byte*)outmsg, ll / 16 + 1, 16, true);
		fprintf(f, "|%s\n", msg);
	}


	printf("\r");
	printf("Test 4\r");
	printf("======\r\r");	
	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy(key, "#@!^23432$#@?23jqpbVTY$,");
		strcpy(msg, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\0");
		msg[notest] = '\0';

		uint ll = strlen(msg);

		micros_setup();
		micros();
		time0 = micros();
		aes192_crypt((byte*)msg, (byte*)key, (byte*)outmsg, ll);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]|", notest, time0, time1, delta0);
		NumTools_printArray_b2((byte*)outmsg, ll / 16 + 1, 16, true);
		printf("|%s\n", msg);

		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]|", notest, time0, time1, delta0);
		NumTools_printArray_b2f(f, (byte*)outmsg, ll / 16 + 1, 16, true);
		fprintf(f, "|%s\n", msg);
	}


	printf("\r");
	printf("Test 5\r");
	printf("======\r\r");	
	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy(key, "r@a5PB3rR1_p1_P!c0_Tw20=");
		strcpy(msg, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\0");
		msg[notest] = '\0';

		uint ll = strlen(msg);

		micros_setup();
		micros();
		time0 = micros();
		aes192_crypt((byte*)msg, (byte*)key, (byte*)outmsg, ll);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]|", notest, time0, time1, delta0);
		NumTools_printArray_b2((byte*)outmsg, ll / 16 + 1, 16, true);
		printf("|%s\n", msg);

		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]|", notest, time0, time1, delta0);
		NumTools_printArray_b2f(f, (byte*)outmsg, ll / 16 + 1, 16, true);
		fprintf(f, "|%s\n", msg);
	}

	printf("[END]\r");
	fprintf(f, "[END]\r");
	
	fclose(f);
    
	//for(;;){}
	return 0;
}
