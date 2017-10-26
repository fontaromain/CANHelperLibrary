// Includes
#include "CANHelperLibrary/Connector/MCPCAN/CMCPCANConnector.h"
#include "CANHelperLibrary/Frame/OBD/COBDFrame.h"
#include "CANHelperLibrary/Frame/OBD/TOYOTA/COBDTOYOTADefines.h"
#include "CANHelperLibrary/Frame/CAN/CReadCANFrame.h"

// Declare the specific connector we want to use
CMCPCANConnector S_CAN ;

// OBD frames we want to use
OBD_ENGINE_RPM_FRM(F_ENGINE_RPM) ;
OBD_TOYOTA_FRS_OIL_TEMP(F_OIL_TEMP) ;

// A generic message used for read
CReadCANFrame F_READ_DATA ;

/*****************************************************************************/
void setup()
{
	// Open serial port
	OPEN_SERIAL(115200) ;

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

	// Define filters we want to use : we want to receive messages starting from 0x07D*, 0x07E* and 0x07F* (replies to our requests)
	S_CAN.SetFilter(0, 0, 0x07D00000) ;
	S_CAN.SetFilter(1, 0, 0x07E00000) ;
	S_CAN.SetFilter(2, 0, 0x07F00000) ;
	S_CAN.SetMask(0, 0, 0x07F00000) ;
	S_CAN.SetMask(1, 0, 0x07F00000) ;
	S_CAN.SetMask(2, 0, 0x07F00000) ;

	// Filters set !
	PRINTLN("CAN filters initialization OK") ;
}

/*****************************************************************************/
void loop()
{
	// Send messages we want to have updates for
	F_ENGINE_RPM.SendAndUpdate(S_CAN, F_READ_DATA) ;
	F_OIL_TEMP.SendAndUpdate(S_CAN, F_READ_DATA) ;

	//////////////////////////////////////
	// Warn oil temperature logic
	//////////////////////////////////////

	// Oil too cold ?
	if (F_OIL_TEMP.GetCurrentValue() <= 85)
	{
		// Above 4k rpm ?
		if (F_ENGINE_RPM.GetCurrentValue() > 4500)
		{
			// Warn user
		}
		else
		{
			// Todo
		}
	}
	// Oil normal condition ?
	else if (F_OIL_TEMP.GetCurrentValue() <= 110)
	{
		// Todo !
	}
	// Oil almost too hot ?
	else if (F_OIL_TEMP.GetCurrentValue() <= 120)
	{
		// Warn user
	}
	// Oil too hot
	else
	{
		// Warn user
	}

	// Wait some time before trying again
	DELAY(100) ;
}