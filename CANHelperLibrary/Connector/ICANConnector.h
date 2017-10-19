#pragma once

// Includes
#include "../CGlobals.h"

// Class debug defines
#define ICANCONNECTOR_DEBUG_SEND
#define ICANCONNECTOR_DEBUG_RECEIVE

/**
 *	@class ICANConnector
 *	Interface through CAN library
 */
class ICANConnector
{
protected:
	/**
	 *	Destructor
	 */
	virtual ~ICANConnector()
	{
	}

public:
	/**
	 *	Initialize the CAN bus
	 *	@return True on success, false otherwise
	 */
	virtual bool Initialize() = 0 ;

	/**
	 *	Sets a specific mask
	 *	@param[in]  pId			Mask Id
	 *	@param[in]	pFrameType	Standard or extended frame
	 *	@param[in]  pMask		Mask to use
	 *	@return True on success, false otherwise
	 */
	virtual bool SetMask(unsigned char pId, unsigned char pFrameType, unsigned long pMask) = 0 ;

	/**
	 *	Sets a specific filter
	 *	@param[in]  pId			Filter Id
	 *	@param[in]	pFrameType	Standard or extended frame
	 *	@param[in]  pFilter		Filter to use
	 *	@return True on success, false otherwise
	 */
	virtual bool SetFilter(unsigned char pId, unsigned char pFrameType, unsigned long pFilter) = 0 ;

	/**
	 *	Tests if there is something to read or not
	 *	@return True if something, false otherwise
	 */
	virtual bool HasMessages() = 0 ;

	/**
	 *	Reads a frame
	 *	@param[out] pCANId	Read CAN Id
	 *	@param[out] pLength	Read data length
	 *	@param[out] pData	Read data
	 *	@return True on success, false otherwise
	 */
	bool Read(unsigned long& pCANId, unsigned char& pLength, unsigned char* pData)
	{
		// Read worked ?
		if (this->ReadImpl(pCANId, pLength, pData))
		{
			#ifdef ICANCONNECTOR_DEBUG_RECEIVE
				// Print that we've just received a frame
				PRINT("Received : ") ;

				// Print frame
				this->Print(pCANId, pLength, pData) ;
			#endif

			// Send succeeded
			return true ;
		}
		else
		{
			#ifdef ICANCONNECTOR_DEBUG_RECEIVE
				// Print error
				PRINTLN("Receive failed") ;
			#endif

			// Failed !
			return false ;
		}
	}

	/**
	 *	Sends a frame
	 *	@param[in] pCANId	CAN Id to use
	 *	@param[in] pLength	Data length
	 *	@param[in] pData	Data to send
	 *	@return True on success, false otherwise
	 */
	bool Send(unsigned long pCANId, unsigned char pLength, const unsigned char* const pData)
	{
		// Send worked ?
		if (this->SendImpl(pCANId, pLength, pData))
		{
			#ifdef ICANCONNECTOR_DEBUG_SEND
				// Print that we've just sent a frame
				PRINT("Sent : ") ;

				// Print frame
				this->Print(pCANId, pLength, pData) ;
			#endif

			// Send succeeded
			return true ;
		}
		else
		{
			#ifdef ICANCONNECTOR_DEBUG_SEND
				// Print error
				PRINTLN("Send failed") ;
			#endif

			// Failed !
			return false ;
		}
	}

protected:
	/**
	 *	Reads a frame implementation
	 *	@param[out] pCANId      Message CAN Id
	 *	@param[out] pDataLenght	Read data length
	 *	@param[out] pReadData	Read data
	 *	@return True on success, false otherwise
	 */
	virtual bool ReadImpl(unsigned long& pCANId, unsigned char& pDataLength, unsigned char* pReadData) = 0 ;

	/**
	 *	Sends a frame implementation
	 *	@param[in] pCANId	CAN Id to use
	 *	@param[in] pLength	Data length
	 *	@param[in] pData	Data to send
	 *	@return True on success, false otherwise
	 */
	virtual bool SendImpl(unsigned long pCANId, unsigned char pLength, const unsigned char* const pData) = 0 ;

protected:
	/**
	 *	Prints the frame content
	 *	@param[in] pCANId	CAN Id to use
	 *	@param[in] pLength	Data length
	 *	@param[in] pData	Data to send
	 */
	void Print(unsigned long pCANId, unsigned char pLength, const unsigned char* const pData) const
	{
		// Print address first
		PRINT("0x") ;
		PRINTHEX(pCANId) ;
		PRINT("\t") ;

		// Then each data
		for (int lDataIndex = 0 ; lDataIndex < pLength ; lDataIndex++)
		{
			PRINT(pData[lDataIndex] <= 0xF ? " 0x0" : " 0x") ;
			PRINTHEX(pData[lDataIndex]) ;
		}

		// And go next line
		PRINTLN() ;
	}
} ;