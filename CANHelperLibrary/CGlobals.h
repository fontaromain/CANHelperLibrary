#pragma once

#ifdef ARDUINO_AVR_UNO
	// Use Arduino IDE Serial
	#define OPEN_LOG() 			Serial.begin(115200)
	#define PRINT(pArg)         Serial.print(pArg)
	#define PRINTLN(pArg)       Serial.println(pArg)
	#define PRINTHEX(pArg)      Serial.print(pArg, HEX)
	#define PRINTLNHEX(pArg)    Serial.println(pArg, HEX)
	#define DELAY(pMs)          delay(pMs)
	#define MILLIS()            millis()
#else
	// Specific includes
	#include <chrono>

	// Define yout specific logging methods
	#define OPEN_LOG()
	#define PRINT(pArg)
	#define PRINTLN(pArg)
	#define PRINTHEX(pArg)
	#define PRINTLNHEX(pArg)
	#define DELAY(pMs)
	#define MILLIS() static_cast<unsigned long>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count())
#endif

/*! \mainpage CAN Helper library
 *
 * \section intro_sec Introduction
 *
 *	This project aims to offer a simple interface through CAN network over Arduino (or other systems).
 *	Use it at your own risks.
 *
 * \section misc_sec Misc
 *
 *	If you need to use a different CAN BUS shield than the one I used, simply create your specific connector using the provided ICANConnector interface.
 *	You are welcome to send it to me so I can add it to the connectors list ;)
 *
 *	For the record, I used the MCP CAN library that you could find in the subfolders, with Arduino IDE 1.8.2, a classic Arduino UNO and the CAN BUS Shield from http://wiki.seeed.cc/CAN-BUS_Shield_V1.2/
 *
 *	Have fun !
 */