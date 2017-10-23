#pragma once

#ifdef ARDUINO_AVR_UNO
	// Use Arduino IDE Serial
	#define PRINT(pArg)			Serial.print(pArg)
	#define PRINTLN(pArg)		Serial.println(pArg)
	#define PRINTHEX(pArg)		Serial.print(pArg, HEX)
	#define PRINTLNHEX(pArg)	Serial.println(pArg, HEX)
	#define DELAY(pMs)          delay(pMs)
	#define OPEN_SERIAL(pBauds) Serial.begin(pBauds) ;
#else
	// Define yout specific logging methods
	#define PRINT(pArg)
	#define PRINTLN(pArg)
	#define PRINTHEX(pArg)
	#define PRINTLNHEX(pArg)
	#define DELAY(pMs)
	#define OPEN_SERIAL(pBauds)
#endif