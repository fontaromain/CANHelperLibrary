// Includes
#include <avr/wdt.h> // Watchdog security
#include "./CANHelperLibrary/Connector/MCPCAN/CMCPCANConnector.h"
#include "./CANHelperLibrary/Modules/TOYOTA/FRS/CCloseDoors.h"

// Declare the specific connector we want to use
CAN::CMCPCANConnector S_CAN ;

// A generic message used for read
CAN::CReadCANFrame F_READ_DATA ;

// Modules to use
FRS::CCloseDoors S_DOORS_LOCK(S_CAN, F_READ_DATA) ;

/*****************************************************************************/
void setup()
{
	// Wait some time for initialization to be always correct (if powered with ACC or IGN only)
	DELAY(3000) ;
	
	// Open log
	OPEN_LOG() ;

	// While CAN initialization fails
	while (!S_CAN.Initialize())
	{
		// Tell user
		PRINTLN_STR("CAN initialization failed") ;

		// Wait some time before trying again
		DELAY(100) ;
	}

	// Ok, everything seems good
	PRINTLN_STR("CAN initialization OK") ;

	// Define filters we want to use
	S_CAN.SetFilter(0, 0, 0x07500000) ;
	S_CAN.SetFilter(1, 0, 0x07C00000) ;
	S_CAN.SetFilter(2, 0, 0x07D00000) ;
	S_CAN.SetFilter(3, 0, 0x07E00000) ;
	S_CAN.SetFilter(4, 0, 0x07F00000) ;
	S_CAN.SetMask(0, 0, 0x07F00000) ;
	S_CAN.SetMask(1, 0, 0x07F00000) ;
	S_CAN.SetMask(2, 0, 0x07F00000) ;
	S_CAN.SetMask(3, 0, 0x07F00000) ;
	S_CAN.SetMask(4, 0, 0x07F00000) ;

	// Filters set !
	PRINTLN_STR("CAN filters initialization OK") ;
}

/*****************************************************************************/
void loop()
{
	// Setup the watchdog
	wdt_enable(WDTO_8S) ;
	
	// Update close door module
	S_DOORS_LOCK.Update(MILLIS()) ;

	// Reset watchdog
	wdt_reset() ;
}