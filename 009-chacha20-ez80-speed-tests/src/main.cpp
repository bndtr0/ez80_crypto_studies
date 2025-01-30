/*******************************************************************************
	[2024/09/14 18:42 NZT]
	009-chacha20-ez80-speed-tests
	(using eZ80 CEdev/AgDev C/C++ environment in GNU/Linux for Agon Light 2 microcomputer)
	
	* ChaCha20 implemented-in-software version.
	* Zilog eZ80F92 CPU calculated different ChaCha20 encryptions over different given strings
	    and displays the original text with it ChaCha20 encryption directly in a text file inside
	    the SD card filesystem (for the Agon Light 2 microcomputer).
	* 1000 tests.
********************************************************************************/
#include <stdio.h>
#include <string.h>
#include "chacha20.h"
#include "numtools.h"

const char s0[] = "The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.\0";
const char s1[] = "abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvw\0";
const char s2[] = "0123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedc\0";
const char s3[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\0";
const char s4[] = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\0";

char msg[1100], key[33], outmsg[2200];



int main()
{
	uint time0, time1;
	uint delta0;

	FILE *f = fopen("log.txt", "wt");


	printf(" Agon Light 2 (Zilog eZ80F92 Acclaim! CPU)\n");
	printf("[ 2024/09/14 18:42 NZT]\n");
	printf("[ ChaCha20 encryption test over different strings ]\n");
	printf("[  -- measure of time used for ChaCha20 crypt --  ]\n");
	printf("[      -- over different string lengths  --       ]\n");
	printf("===================================================\n\n");

	fprintf(f, " Agon Light 2 (Zilog eZ80F92 Acclaim! CPU)\n");
	fprintf(f, "[ 2024/09/14 18:42 NZT]\n");
	fprintf(f, "[ ChaCha20 encryption test over different strings ]\n");
	fprintf(f, "[  -- measure of time used for ChaCha20 crypt --  ]\n");
	fprintf(f, "[      -- over different string lengths  --       ]\n");
	fprintf(f, "==============================================\n\n");



	printf("Test 1\n");
	printf("======\n\n");
	fprintf(f, "Test 1\n");
	fprintf(f, "======\n\n");

	for(int notest = 1; notest <= 1010; notest++)
	{
		// The "key" for encryption can have a maximum of 32 bytes (4 32-bits words).
		strcpy(key, "12345678901234567890123456789012");

		// The "nonce" always have 12 bytes for this algorithm (3 32-bits words).
		byte n[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	
		strcpy(msg, s0);
		msg[notest] = '\0';
		
		uint ll = strlen(msg);

		micros_setup();
		micros();
		time0 = micros();
		chacha20_crypt(msg, key, n, 0x00, (byte*)outmsg, ll, 32, true);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]|%s|(%d)|", notest, time0, time1, delta0, msg, strlen(msg));
		NumTools_printArray_b((byte*)outmsg, notest, 1, true, true, false);
		printf("\n");
		
		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]|%s|(%d)|", notest, time0, time1, delta0, msg, strlen(msg));
		NumTools_printArray_bf(f, (byte*)outmsg, notest, 1, true, true, false);
		fprintf(f, "\n");
	}
	
	printf("\n\n");


	printf("Test 2\n");
	printf("======\n\n");
	fprintf(f, "Test 2\n");
	fprintf(f, "======\n\n");

	for(int notest = 1; notest <= 1010; notest++)
	{
		// The "key" for encryption can have a maximum of 32 bytes (4 32-bits words).
		strcpy(key, "lorem_ipsum_2024_09_25_PM_labor_");

		// The "nonce" always have 12 bytes for this algorithm (3 32-bits words).
		byte n[] = { 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff };
	
		strcpy(msg, s1);
		msg[notest] = '\0';
		uint ll = strlen(msg);

		micros_setup();
		micros();
		time0 = micros();
		chacha20_crypt(msg, key, n, 0xa0, (byte*)outmsg, ll, 32, true);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]|%s|(%d)|", notest, time0, time1, delta0, msg, strlen(msg));
		NumTools_printArray_b((byte*)outmsg, notest, 1, true, true, false);
		printf("\n");
		
		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]|%s|(%d)|", notest, time0, time1, delta0, msg, strlen(msg));
		NumTools_printArray_bf(f, (byte*)outmsg, notest, 1, true, true, false);
		fprintf(f, "\n");
	}
	
	printf("\n\n");




	printf("Test 3\n");
	printf("======\n\n");
	fprintf(f, "Test 3\n");
	fprintf(f, "======\n\n");

	for(int notest = 1; notest <= 1010; notest++)
	{
		// The "key" for encryption can have a maximum of 32 bytes (4 32-bits words).
		strcpy(key, "2024_08_16_14-47_PM_NZT-UTF-+12.");

		// The "nonce" always have 12 bytes for this algorithm (3 32-bits words).
		byte n[] = { 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00 };
	
		strcpy(msg, s2);
		msg[notest] = '\0';
		uint ll = strlen(msg);

		micros_setup();
		micros();
		time0 = micros();
		chacha20_crypt(msg, key, n, 0xa0, (byte*)outmsg, ll, 32, true);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]|%s|(%d)|", notest, time0, time1, delta0, msg, strlen(msg));
		NumTools_printArray_b((byte*)outmsg, notest, 1, true, true, false);
		printf("\n");
		
		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]|%s|(%d)|", notest, time0, time1, delta0, msg, strlen(msg));
		NumTools_printArray_bf(f, (byte*)outmsg, notest, 1, true, true, false);
		fprintf(f, "\n");
	}
	
	printf("\n\n");



	printf("Test 4\n");
	printf("======\n\n");
	fprintf(f, "Test 4\n");
	fprintf(f, "======\n\n");

	for(int notest = 1; notest <= 1010; notest++)
	{
		// The "key" for encryption can have a maximum of 32 bytes (4 32-bits words).
		strcpy(key, "#@!^23432$#@?23jqpbVTY$,dpBSK4;=");

		// The "nonce" always have 12 bytes for this algorithm (3 32-bits words).
		byte n[] = { 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00 };
	
		strcpy(msg, s3);
		msg[notest] = '\0';
		uint ll = strlen(msg);

		micros_setup();
		micros();
		time0 = micros();
		chacha20_crypt(msg, key, n, 0xa0, (byte*)outmsg, ll, 32, true);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]|%s|(%d)|", notest, time0, time1, delta0, msg, strlen(msg));
		NumTools_printArray_b((byte*)outmsg, notest, 1, true, true, false);
		printf("\n");
		
		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]|%s|(%d)|", notest, time0, time1, delta0, msg, strlen(msg));
		NumTools_printArray_bf(f, (byte*)outmsg, notest, 1, true, true, false);
		fprintf(f, "\n");
	}
	
	printf("\n\n");



	printf("Test 5\n");
	printf("======\n\n");
	fprintf(f, "Test 5\n");
	fprintf(f, "======\n\n");

	for(int notest = 1; notest <= 1010; notest++)
	{
		// The "key" for encryption can have a maximum of 32 bytes (4 32-bits words).
		strcpy(key, "r@a5PB3rR1_p1_P!c0_Tw20=Rp2Es0+*");

		// The "nonce" always have 12 bytes for this algorithm (3 32-bits words).
		byte n[] = { 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00 };
	
		strcpy(msg, s4);
		msg[notest] = '\0';
		uint ll = strlen(msg);

		micros_setup();
		micros();
		time0 = micros();
		chacha20_crypt(msg, key, n, 0xa0, (byte*)outmsg, ll, 32, true);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]|%s|(%d)|", notest, time0, time1, delta0, msg, strlen(msg));
		NumTools_printArray_b((byte*)outmsg, notest, 1, true, true, false);
		printf("\n");
		
		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]|%s|(%d)|", notest, time0, time1, delta0, msg, strlen(msg));
		NumTools_printArray_bf(f, (byte*)outmsg, notest, 1, true, true, false);
		fprintf(f, "\n");
	}
	
	printf("\n\n");

	printf("[END]\n\n");
	fprintf(f, "[END]\n\n");

	fclose(f);

	return 0;
}

