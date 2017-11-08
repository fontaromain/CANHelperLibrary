// Includes
#include <avr/wdt.h> // Watchdog security
#include "./CANHelperLibrary/Connector/MCPCAN/CMCPCANConnector.h"
#include "./CANHelperLibrary/Modules/TOYOTA/FRS/COilDisplay.h"

// Declare the specific connector we want to use
CAN::CMCPCANConnector S_CAN ;

// Modules to use
FRS::COilDisplay S_OIL_DISPLAY ;

// A generic message used for read
CAN::CReadCANFrame F_READ_DATA ;

// Global status variables
unsigned long                   S_DISPLAY_BUTTON_PRESS_START = 0 ;	// Time when the user started to press the cluster display button
FRS::CQueryCombiButtonsFrame	S_COMBI_BTNS ;						// Frame to query combi buttons status

/*****************************************************************************/
void setup()
{
	// Open log
	OPEN_LOG() ;

	// While CAN initialization fails
	while (!S_CAN.Initialize())
	{
		// Tell user
		PRINTLN("CAN initialization failed") ;

		// Wait some time before trying again
		DELAY(100) ;
	}

	// Ok, everything seems good
	PRINTLN("CAN initialization OK") ;

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
	PRINTLN("CAN filters initialization OK") ;
}

/*****************************************************************************/
void loop()
{
	// Setup the watchdog
	wdt_enable(WDTO_8S) ;
	
	// Query current time only once
	unsigned long lCurrentTime = MILLIS() ;
	
	// Update oil show module
	S_OIL_DISPLAY.Update(lCurrentTime, S_CAN, F_READ_DATA) ;

	// Look for activation / desactivation of the oil temperature display
	if (S_COMBI_BTNS.SendAndUpdate(S_CAN, F_READ_DATA))
	{
		// The display button is pressed ?
		if (S_COMBI_BTNS.GetCurrentValue() == FRS::ECombiBtnStatus::S_DISPLAY_PRESSED)
		{
			// Check if elapsed time is enough to activate the oil display. Enough ?
			if (lCurrentTime - S_DISPLAY_BUTTON_PRESS_START > 2000)
			{
				// Activate or desactivate oil display module
				if (S_OIL_DISPLAY.IsEnabled())
				{
					S_OIL_DISPLAY.Disable(lCurrentTime, S_CAN, F_READ_DATA) ;
				}
				else
				{
					S_OIL_DISPLAY.Enable(lCurrentTime, S_CAN, F_READ_DATA) ;
				}

				// Reset time counter
				S_DISPLAY_BUTTON_PRESS_START = lCurrentTime ;
			}
		}
		else
		{
			// Reset time counter
			S_DISPLAY_BUTTON_PRESS_START = MILLIS() ;
		}
	}
	else
	{
		// Reset time counter
		S_DISPLAY_BUTTON_PRESS_START = MILLIS() ;
	}
	
	// Reset watchdog
	wdt_reset() ;
}