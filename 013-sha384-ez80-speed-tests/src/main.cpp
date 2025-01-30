/*******************************************************************************
	[2024/09/13 12:39 NZT]
	013-sha384-ez80-speed-tests
	(using eZ80 CEdev/AgDev C/C++ environment in GNU/Linux for Agon Light 2 microcomputer)
	
	* SHA-384-implemented-in-software version.
	* Zilog eZ80F92 CPU calculated different SHA-384 hashes over different given strings
	    and displays the original text with it SHA-384 hash directly in a text file inside
	    the SD card filesystem (for the Agon Light 2 microcomputer).
	* 1000 tests.
********************************************************************************/
#include <stdio.h>
#include <string.h>
#include "sha384.h"
#include "numtools.h"

const char s0[] = "The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.\0";
const char s1[] = "abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvw\0";
const char s2[] = "0123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedc\0";
const char s3[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\0";
const char s4[] = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\0";

char msg[1100];

void  micros_setup();

int main()
{
	ulong time0, time1;
	ulong delta0;

	FILE *f = fopen("log1.txt", "wt");


	printf(" Agon Light 2 (Zilog eZ80F92 Acclaim! CPU)\n");
	printf("[2024/09/13 12:39 NZT]\n");
	printf("[  SHA-384 hashing test calculation over different strings  ]\n");
	printf("[   -- measure of time used for calculation of SHA-384 --   ]\n");
	printf("[            -- over different string lengths  --           ]\n");
	printf("=============================================================\n\n");

	fprintf(f, " Agon Light 2 (Zilog eZ80F92 Acclaim! CPU)\n");
	fprintf(f, "[2024/09/13 12:39 NZT]\n");
	fprintf(f, "[  SHA-384 hashing test calculation over different strings  ]\n");
	fprintf(f, "[   -- measure of time used for calculation of SHA-384 --   ]\n");
	fprintf(f, "[            -- over different string lengths  --           ]\n");
	fprintf(f, "=============================================================\n\n");



	printf("Test 1\n");
	printf("======\n\n");
	fprintf(f, "Test 1\n");
	fprintf(f, "======\n\n");

	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy(msg, s0);
		msg[notest] = '\0';

		micros_setup();
		micros();
		time0 = micros();
		char *sha384_output = sha384_hash((byte*)msg, notest);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]||%s|%s|(%d)\n", notest, time0, time1, delta0, sha384_output, msg, strlen(msg));
		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]||%s|%s|(%d)\n", notest, time0, time1, delta0, sha384_output, msg, strlen(msg));
	}
	
	printf("\n\n");


	printf("Test 2\n");
	printf("======\n\n");
	fprintf(f, "Test 2\n");
	fprintf(f, "======\n\n");


	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy(msg, s1);
		msg[notest] = '\0';

		micros_setup();
		micros();
		time0 = micros();
		char *sha384_output = sha384_hash((byte*)msg, notest);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]||%s|%s|(%d)\n", notest, time0, time1, delta0, sha384_output, msg, strlen(msg));
		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]||%s|%s|(%d)\n", notest, time0, time1, delta0, sha384_output, msg, strlen(msg));
	}

	printf("\n\n");



	printf("Test 3\n");
	printf("======\n\n");
	fprintf(f, "Test 3\n");
	fprintf(f, "======\n\n");


	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy(msg, s2);
		msg[notest] = '\0';

		micros_setup();
		micros();
		time0 = micros();
		char *sha384_output = sha384_hash((byte*)msg, notest);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]||%s|%s|(%d)\n", notest, time0, time1, delta0, sha384_output, msg, strlen(msg));
		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]||%s|%s|(%d)\n", notest, time0, time1, delta0, sha384_output, msg, strlen(msg));
	}

	printf("\n\n");



	printf("Test 4\n");
	printf("======\n\n");
	fprintf(f, "Test 4\n");
	fprintf(f, "======\n\n");


	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy(msg, s3);
		msg[notest] = '\0';

		micros_setup();
		micros();
		time0 = micros();
		char *sha384_output = sha384_hash((byte*)msg, notest);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]||%s|%s|(%d)\n", notest, time0, time1, delta0, sha384_output, msg, strlen(msg));
		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]||%s|%s|(%d)\n", notest, time0, time1, delta0, sha384_output, msg, strlen(msg));
	}

	printf("\n\n");



	printf("Test 5\n");
	printf("======\n\n");
	fprintf(f, "Test 5\n");
	fprintf(f, "======\n\n");


	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy(msg, s4);
		msg[notest] = '\0';

		micros_setup();
		micros();
		time0 = micros();
		char *sha384_output = sha384_hash((byte*)msg, notest);
		time1 = micros();
		micros_stop();

		delta0 = (time0 - time1) * 14UL;	// each tick in TIMER5 is (18.432 [MHz])^-1 = 13.888 [us] ~ 14 [us]

		printf("%d |%6.6lx|%6.6lx|%lu [us]||%s|%s|(%d)\n", notest, time0, time1, delta0, sha384_output, msg, strlen(msg));
		fprintf(f, "%d |%6.6lx|%6.6lx|%lu [us]||%s|%s|(%d)\n", notest, time0, time1, delta0, sha384_output, msg, strlen(msg));
	}

	printf("\n\n");


	printf("[END]\n\n");
	fprintf(f, "[END]\n\n");

	fclose(f);

	return 0;
}

