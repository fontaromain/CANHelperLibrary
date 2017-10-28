// Includes
#include "CANHelperLibrary/Connector/MCPCAN/CMCPCANConnector.h"
#include "CANHelperLibrary/Frame/OBD/COBDFrame.h"
#include "CANHelperLibrary/Frame/OBD/TOYOTA/COBDTOYOTADefines.h"
#include "CANHelperLibrary/Frame/CAN/CReadCANFrame.h"

// Declare the specific connector we want to use
CMCPCANConnector S_CAN ;

// OBD frames we want to use
OBD_ENGINE_RPM_FRM(F_ENGINE_RPM) ;
OBD_VEHICLE_SPEED_FRM(F_VEHICLE_SPEED) ;
OBD_TOYOTA_FRS_LOCK_DOORS(F_LOCK_DOORS) ;
OBD_TOYOTA_FRS_UNLOCK_DOORS(F_UNLOCK_DOORS) ;

// A generic message used for read
CReadCANFrame F_READ_DATA ;

// Global status variables
bool S_DOORS_LOCKED = false ; // Current door lock status

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
	S_CAN.SetFilter(0, 0, 0x07C00000) ;
	S_CAN.SetFilter(1, 0, 0x07D00000) ;
	S_CAN.SetFilter(2, 0, 0x07E00000) ;
	S_CAN.SetFilter(3, 0, 0x07F00000) ;
	S_CAN.SetMask(0, 0, 0x07F00000) ;
	S_CAN.SetMask(1, 0, 0x07F00000) ;
	S_CAN.SetMask(2, 0, 0x07F00000) ;
	S_CAN.SetMask(3, 0, 0x07F00000) ;

	// Filters set !
	PRINTLN("CAN filters initialization OK") ;
}

/*****************************************************************************/
void loop()
{
	// Send messages we want updates for
	F_ENGINE_RPM.SendAndUpdate(S_CAN, F_READ_DATA) ;
	F_VEHICLE_SPEED.SendAndUpdate(S_CAN, F_READ_DATA) ;

	//////////////////////////////////////
	// Close door logic
	//////////////////////////////////////

	// Doors are currently locked ?
	if (S_DOORS_LOCKED)
	{
		// Engine stopped ?
		if (F_ENGINE_RPM.GetCurrentValue() == 0)
		{
			// Try to unlock doors. Succeeded ?
			if (F_UNLOCK_DOORS.SendTo(S_CAN))
			{
				// Doors are now unlocked
				S_DOORS_LOCKED = false ;
			}
		}
	}
	// Doors currently not locked
	else
	{
		// Vehicle speed reached the close limit ?
		if (F_VEHICLE_SPEED.GetCurrentValue() > 15)
		{
			// Try to lock doors. Succeeded ?
			if (F_LOCK_DOORS.SendTo(S_CAN))
			{
				// Doors are now locked
				S_DOORS_LOCKED = true ;
			}
		}
	}

	// Wait some time before trying again
	DELAY(100) ;
}