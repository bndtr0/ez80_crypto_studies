/*******************************************************************************
	[2024/08/24 19:00 NZT]
	008-rc4-ez80-speed-tests
	(using eZ80 CEdev/AgDev C/C++ environment in GNU/Linux for Agon Light 2 microcomputer)
	
	* RC4-implemented-in-software version.
	* Zilog eZ80F92 CPU calculated different RC4 encryptions over different given strings
	    and displays the original text with it RC4 encryption directly in a text file inside
	    the SD card filesystem (for the Agon Light 2 microcomputer).
	* 1000 tests.
********************************************************************************/
#include <stdio.h>
#include <string.h>
#include "rc4.h"
#include "numtools.h"

const char s0[] = "The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.\0";
const char s1[] = "abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvw\0";
const char s2[] = "0123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedc\0";
const char s3[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\0";
const char s4[] = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\0";

char msg[1100], key[64], outmsg[2200];



int main()
{
	ulong time0, time1;
	ulong delta0;

	FILE *f = fopen("log.txt", "wt");


	printf(" Agon Light 2 (Zilog eZ80F92 Acclaim! CPU)\n");
	printf("[ 2024/08/24 19:00 NZT]\n");
	printf("[ RC4 encryption test over different strings ]\n");
	printf("[  -- measure of time used for RC4 crypt --  ]\n");
	printf("[    -- over different string lengths  --    ]\n");
	printf("==============================================\n\n");

	fprintf(f, " Agon Light 2 (Zilog eZ80F92 Acclaim! CPU)\n");
	fprintf(f, "[ 2024/08/24 19:00 NZT]\n");
	fprintf(f, "[ RC4 encryption test over different strings ]\n");
	fprintf(f, "[  -- measure of time used for RC4 crypt --  ]\n");
	fprintf(f, "[    -- over different string lengths  --    ]\n");
	fprintf(f, "==============================================\n\n");



	printf("Test 1\n");
	printf("======\n\n");
	fprintf(f, "Test 1\n");
	fprintf(f, "======\n\n");

	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy(key, "123456");
		strcpy(msg, s0);
		msg[notest] = '\0';
		
		micros_setup();
		micros();
		time0 = micros();
		rc4_crypt((byte*)msg, (byte*)key, (byte*)outmsg, strlen(msg), strlen(key));
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
		strcpy(key, "lorem_ipsum_2024");
		strcpy(msg, s1);
		msg[notest] = '\0';
		
		micros_setup();
		micros();
		time0 = micros();
		rc4_crypt((byte*)msg, (byte*)key, (byte*)outmsg, strlen(msg), strlen(key));
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
		strcpy(key, "2024_08_16_14-47.NZT.localT1M3");
		strcpy(msg, s2);
		msg[notest] = '\0';
		
		micros_setup();
		micros();
		time0 = micros();
		rc4_crypt((byte*)msg, (byte*)key, (byte*)outmsg, strlen(msg), strlen(key));
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
		strcpy(key, "#@!^23432$#@?23jkl43jk4l234j_dfsddsDSFSFEWrQLOB");
		strcpy(msg, s3);
		msg[notest] = '\0';
		
		micros_setup();
		micros();
		time0 = micros();
		rc4_crypt((byte*)msg, (byte*)key, (byte*)outmsg, strlen(msg), strlen(key));
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
		strcpy(key, "r@a5PB3rR1p1P!c0");
		strcpy(msg, s4);
		msg[notest] = '\0';
		
		micros_setup();
		micros();
		time0 = micros();
		rc4_crypt((byte*)msg, (byte*)key, (byte*)outmsg, strlen(msg), strlen(key));
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

