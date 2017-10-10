#pragma once

// Includes
#include "./CStdDataFrame.h"
#include "../../Connector/ICANConnector.h"

/**
 *	@class CStdDataFrameReader
 *	A frame used to read CAN connector messages
 */
class CStdDataFrameReader : public CStdDataFrame
{
public:	
	/**
	 *	Reads data from the given reader
	 *	@param[in] pCAN CAN reader to use
	 *	@return True on success, false otherwise
	 */
	bool ReadFrom(ICANConnector& pCAN)
	{
		// A message available ? And read with sucess ?
		if (pCAN.HasMessages())
		{
			// Read succeed ?
			if (pCAN.Read(this->mAddress, this->mLength, this->mData))
			{
#ifdef DEBUG_RECEIVE
				// Print that we've just received a message
				Serial.print("Received : ") ;

				// Print message
				this->Print() ;
#endif
				// Receive succeeded
				return true ;
			}
#ifdef DEBUG_RECEIVE	
			// Print error
			Serial.println("Receive failed") ;
#endif
		}
	
		// Error
		return false ;
	}

protected:
	unsigned char mLength ;	/**< Data length */
} ;