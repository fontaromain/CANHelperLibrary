#pragma once

// Includes
#include "./CStdDataFrame.h"
#include "../../Connector/ICANConnector.h"

/**
 *	@class CSendStdDataFrame
 *	Standard CAN data frame that could be sent over CAN network
 *	Used to simplify data creation / usage
 */
class CSendStdDataFrame : public CStdDataFrame
{
public:
	/**
	 *	Constructor
	 *	@param[in] pAddress Frame address
	 *	@param[in] pD0		Data 0
	 *	@param[in] pD1		Data 1
	 *	@param[in] pD2		Data 2
	 *	@param[in] pD3		Data 3
	 *	@param[in] pD4		Data 4
	 *	@param[in] pD5		Data 5
	 *	@param[in] pD6		Data 6
	 *	@param[in] pD7		Data 6
	 */
	CSendStdDataFrame(unsigned long pAddress, unsigned char pD0 = 0x0, unsigned char pD1 = 0x0, unsigned char pD2 = 0x0, unsigned char pD3 = 0x0, unsigned char pD4 = 0x0, unsigned char pD5 = 0x0, unsigned char pD6 = 0x0, unsigned char pD7 = 0x0) 
	{
		// Initialize address 
		this->mAddress = pAddress ;
    
		// Initialize data
		this->mData[0] = pD0 ;
		this->mData[1] = pD1 ;
		this->mData[2] = pD2 ;
		this->mData[3] = pD3 ;
		this->mData[4] = pD4 ;
		this->mData[5] = pD5 ;
		this->mData[6] = pD6 ;
		this->mData[7] = pD7 ;
	}
	
	/**
	 *	Sends data to the given CAN connector
	 *	@param[in] pCAN CAN connector to use
	 *	@return True on success, false otherwise
	 */
	bool SendTo(ICANConnector& pCAN)
	{
		// Send worked ?
		if (pCAN.Send(this->GetAddress(), this->GetLength(), this->GetData()))
		{
#ifdef DEBUG_SEND
			// Print that we've just sent a message
			Serial.print("Sent : ") ;

			// Print message
			this->Print() ;
#endif
			// Send succeeded
			return true ;
	}
	
#ifdef DEBUG_SEND	
		// Print error
		Serial.println("Send failed") ;
#endif
	
		// Error
		return false ;
	}
} ;