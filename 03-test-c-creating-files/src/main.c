/*******************************************************************************
	[2024/08/22 13:31 NZT]
	006-md5-ez80-speed-tests
	(using eZ80 CEdev/AgDev C/C++ environment in GNU/Linux for Agon Light 2 microcomputer)
	
	* MD5-implemented-in-software version.
	* Zilog eZ80F92 CPU calculated different MD5 hashes over different given strings
	    and displays the original text with it MD5 hash directly in a text file inside
	    the SD card filesystem (for the Agon Light 2 microcomputer).
	* 1000 tests.
********************************************************************************/

#include <stdio.h>

int main()
{
	FILE* f = fopen("log1.txt", "wt");
	
	for(int i = 0; i <= 1000; i++)
	{
		fputc('@', f);
		fputc((char)i, f);
	}

	fprintf(f, "[END]\n\n");
	printf("[END]\n\n");

	fclose(f);
	
	return 0;
}
