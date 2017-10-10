// Includes
#include "CANHelperLibrary/Connector/MCPCAN/CMCPCANConnector.h"
#include "CANHelperLibrary/Frame/Standard/CSendStdDataFrameEx.h"
#include "CANHelperLibrary/Frame/Standard/CStdDataFrameReader.h"

// Declare the specific connector we want to use
CMCPCANConnector S_CAN ;

// Generic OBD messages we want to use
CSendStdDataFrameEx<unsigned int>	F_ENGINE_RPM(BROADCAST_ADDR, 0x02, OBD_MODE_1, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, [](const AFrame& pData) { return (unsigned int)(256 * pData.GetData()[3] + pData.GetData()[4]) / 4 ; }) ;
CSendStdDataFrameEx<unsigned char>	F_VEHICLE_SPEED(BROADCAST_ADDR, 0x02, OBD_MODE_1, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, [](const AFrame& pData) { return pData.GetData()[3] ; }) ;

// Specific FRS/BRZ/GT86 OBD messages
CSendStdDataFrame F_LOCK_DOORS(0x750, 0x40, 0x05, 0x30, 0x11, 0x00, 0x80) ;
CSendStdDataFrame F_UNLOCK_DOORS(0x750, 0x40, 0x05, 0x30, 0x11, 0x00, 0x40) ;
	
// A generic message used for read
CStdDataFrameReader F_READ_DATA ;

// Current door lock status
bool S_DOORS_LOCKED = false ;

/*****************************************************************/
/*****************************************************************/
void setup()
{
	// Open serial port
    Serial.begin(115200) ;  

	// While CAN initialization fails
    while (!S_CAN.Initialize())
    {
		// Tell user
        Serial.println("CAN initialization failed") ;
		
		// Wait some time before trying again
        delay(100) ;
    }
	
	// Ok, everything seems good	
    Serial.println("CAN initialization OK") ;
}

/*****************************************************************/
/*****************************************************************/
void loop()
{
	// Send messages we want to have updates for
	F_ENGINE_RPM.SendTo(S_CAN) ;
	F_VEHICLE_SPEED.SendTo(S_CAN) ;
	
	// While there are some received messages
	while (S_CAN.HasMessages())
	{
		// Read current
		F_READ_DATA.ReadFrom(S_CAN) ;
		
		// A reply to one of our request ?
		if (F_READ_DATA.GetAddress() == REPLY_ADDR)
		{
			// Engine RPM ?
			if (F_ENGINE_RPM.IsSameAs(F_READ_DATA))
			{
				// Update message state
				F_ENGINE_RPM(F_READ_DATA) ;
			}
			// Vehicle speed ?
			else if (F_VEHICLE_SPEED.IsSameAs(F_READ_DATA))
			{
				// Update message state
				F_VEHICLE_SPEED(F_READ_DATA) ;
			}
		}
	}
	
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
		if (F_VEHICLE_SPEED.GetCurrentValue() > 18)
		{
			// Try to lock doors. Succeeded ?
			if (F_LOCK_DOORS.SendTo(S_CAN))
			{
				// Doors are now locked
				S_DOORS_LOCKED = true ;
			}
		}
	}
}
