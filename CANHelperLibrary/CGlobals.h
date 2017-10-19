#pragma once

#ifdef FALSE
	#define PRINT(...)          Serial.print(...)
	#define PRINTLN(...)        Serial.println(...)
	#define PRINTHEX(...)       Serial.print(..., HEX)
	#define PRINTLNHEX(...)     Serial.println(..., HEX)
	#define DELAY(pMs)          delay(pMs)
	#define OPEN_SERIAL(pBauds) Serial.begin(pBauds) ;
#else
	// Define yout specific methods
	#define PRINT(...)
	#define PRINTLN(...)
	#define PRINTHEX(...)
	#define PRINTLNHEX(...)
	#define DELAY(pMs)
	#define OPEN_SERIAL(pBauds)
#endif