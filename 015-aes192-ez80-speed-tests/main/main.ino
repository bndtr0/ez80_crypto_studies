/*******************************************************************************
	[2024/08/17 20:41 NZT]
	006-md5-arduino-speed-tests
	(using official Arduino IDE 2.3.2 for GNU/Linux)
	
	* MD5-implemented-in-software version.
	Arduino UNO calculated different MD5 hashes over different given strings and displays the original text with it MD5 hash
	  in /dev/ttyACM0 device (serial-to-USB device into a GNU/Linux terminal).
	1000 tests.
********************************************************************************/
#include <string.h>
#include "md5.h"

#define PIN_ONBOARD_LED 13

void setup() 
{
  Serial.begin(115200);

	pinMode(PIN_ONBOARD_LED, OUTPUT);    // GP25 set as an output to ONBOARD-LED.
}


const PROGMEM char s0[] = "The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.The quick brown fox jumps over the lazy dog.\0";
const PROGMEM char s1[] = "abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz.,;:!@#$^&*()-+=_abcdefghijklmnopqrstuvwxyz\0";
const PROGMEM char s2[] = "0123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba98765432100123456789abcdef0123456789ABCDEFFEDCBA9876543210fedcba9876543210\0";
const PROGMEM char s3[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\0";
const PROGMEM char s4[] = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\0";

void loop()
{
	ulong time0, time1;
	ulong delta0;

	delay(5000);

	Serial.println();
  Serial.println(F("Arduino UNO R3 (ATmega 328p)"));
	Serial.println(F("[2024/08/14 19:54 NZT]"));
  Serial.println();
	Serial.println(F("[MD5 hashing test calculation over different strings]"));
	Serial.println(F("[ -- measure of time used for calculation of MD5 -- ]"));
	Serial.println(F("[        -- over different string lengths  --       ]"));
	Serial.println(F("====================================================="));
  Serial.println();

	char msg[1100];

	/*
	printf("%d = %s\r", 0, NumTools_toHex(0, 1));
	printf("%d = %s\r", 1, NumTools_toHex(1, 2));
	printf("%d = %s\r", 10, NumTools_toHex(10, 3));
	printf("%d = %s\r", 15, NumTools_toHex(15, 4));
	printf("%d = %s\r", 32, NumTools_toHex(32, 5));
	printf("%d = %s\r", 64, NumTools_toHex(64, 6));
	printf("%d = %s\r", 128, NumTools_toHex(128, 7));
	printf("%d = %s\r", 256, NumTools_toHex(256, 8));
	printf("%d = %s\r", 512, NumTools_toHex(512, 8));
	printf("%d = %s\r", 1048575, NumTools_toHex(1048575, 8));
	*/

	Serial.println(F("Test 1"));
	Serial.println(F("======"));
  Serial.println();

	for(int notest = 1; notest <= 1010; notest++)
	{
	  strcpy_P(msg, s0);
    msg[notest] = '\0';

		time0 = micros();
		char *md5_output = md5_hash((byte*)msg, notest);
		time1 = micros();

		delta0 = time1 - time0;

		Serial.print(notest);
    Serial.print("  |");
    Serial.print(delta0);
    Serial.print(" [us]|");
    Serial.print(md5_output);
    Serial.print(F("|"));
    Serial.print(msg);
    Serial.print(F("|"));
    Serial.print(F("("));
    Serial.print(strlen(msg));
    Serial.println(F(")"));
	}



  Serial.println();
	Serial.println(F("Test 2"));
	Serial.println(F("======"));	
  Serial.println();

	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy_P(msg, s1);
    msg[notest] = '\0';

		time0 = micros();
		char *md5_output = md5_hash((byte*)msg, notest);
		time1 = micros();

		delta0 = time1 - time0;

		Serial.print(notest);
    Serial.print("  |");
    Serial.print(delta0);
    Serial.print(" [us]|");
    Serial.print(md5_output);
    Serial.print(F("|"));
    Serial.print(msg);
    Serial.print(F("|"));
    Serial.print(F("("));
    Serial.print(strlen(msg));
    Serial.println(F(")"));
	}

  Serial.println();
	Serial.println(F("Test 3"));
	Serial.println(F("======"));	
  Serial.println();

	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy_P(msg, s2);
    msg[notest] = '\0';

		time0 = micros();
		char *md5_output = md5_hash((byte*)msg, notest);
		time1 = micros();

		delta0 = time1 - time0;

		Serial.print(notest);
    Serial.print("  |");
    Serial.print(delta0);
    Serial.print(" [us]|");
    Serial.print(md5_output);
    Serial.print(F("|"));
    Serial.print(msg);
    Serial.print(F("|"));
    Serial.print(F("("));
    Serial.print(strlen(msg));
    Serial.println(F(")"));
	}


	Serial.println();
	Serial.println(F("Test 4"));
	Serial.println(F("======"));	
  Serial.println();

	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy_P(msg, s3);
    msg[notest] = '\0';

		time0 = micros();
		char *md5_output = md5_hash((byte*)msg, notest);
		time1 = micros();

		delta0 = time1 - time0;

		Serial.print(notest);
    Serial.print("  |");
    Serial.print(delta0);
    Serial.print(" [us]|");
    Serial.print(md5_output);
    Serial.print(F("|"));
    Serial.print(msg);
    Serial.print(F("|"));
    Serial.print(F("("));
    Serial.print(strlen(msg));
    Serial.println(F(")"));
	}


	Serial.println();
	Serial.println(F("Test 5"));
	Serial.println(F("======"));	
  Serial.println();

	for(int notest = 1; notest <= 1010; notest++)
	{
		strcpy_P(msg, s4);
    msg[notest] = '\0';

		time0 = micros();
		char *md5_output = md5_hash((byte*)msg, notest);
		time1 = micros();

		delta0 = time1 - time0;

		Serial.print(notest);
    Serial.print("  |");
    Serial.print(delta0);
    Serial.print(" [us]|");
    Serial.print(md5_output);
    Serial.print(F("|"));
    Serial.print(msg);
    Serial.print(F("|"));
    Serial.print(F("("));
    Serial.print(strlen(msg));
    Serial.println(F(")"));
	}

	Serial.println("[END]");
    
	for(;;){}
}
