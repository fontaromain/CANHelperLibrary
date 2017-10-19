#pragma once

// Includes
#include "../ICANConnector.h"
#include "mcp_can.h"

/**
 *	@class CMCPCANConnector
 *	Interface through CAN library. Useful if you want to use a different one
 */
class CMCPCANConnector : public ICANConnector
{
public:
	/**
	 *	Constructor
	 */
	CMCPCANConnector()
	{
		// Init members
		this->mCAN = new MCP_CAN(10) ; // Build a new MCP_CAN using CS pin 10
	}

	/**
	 *	Destructor
	 */
	virtual ~CMCPCANConnector()
	{
		// Clean up
		delete this->mCAN ;
	}

	/**
	 *	Initialize the CAN bus
	 *	@return True on success, false otherwise
	 */
	virtual bool Initialize() override
	{
		// Initialization ok ?
		if (this->mCAN->begin(MCP_STDEXT, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
		{			
			// Back to normal
			this->mCAN->setMode(MCP_NORMAL) ;
			
			// Succeeded !
			return true ;
		}
		
		// Failed !
		return false ;
	}
	
	/**
	 *	Sets a specific mask
	 *	@param[in] 	pId			Mask Id
	 *	@param[in]	pFrameType	Standard or extended frame
	 *	@param[in] 	pMask		Mask to use
	 *	@return True on success, false otherwise
	 */
	virtual bool SetMask(unsigned char pId, unsigned char pFrameType, unsigned long pMask) override
	{
		return this->mCAN->init_Mask(pId, pFrameType, pMask) == CAN_OK ;
	}
	
	/**
	 *	Sets a specific filter
	 *	@param[in] 	pId			Filter Id
	 *	@param[in]	pFrameType	Standard or extended frame
	 *	@param[in] 	pFilter		Filter to use
	 *	@return True on success, false otherwise
	 */
	virtual bool SetFilter(unsigned char pId, unsigned char pFrameType, unsigned long pFilter) override
	{
		return this->mCAN->init_Filt(pId, pFrameType, pFilter) == CAN_OK ;
	}             
	
	/**
	 *	Tests if there is something to read or not
	 *	@return True if something, false otherwise
	 */
	virtual bool HasMessages() override
	{
		return this->mCAN->checkReceive() == CAN_MSGAVAIL ;
	}

protected:	
	/**
	 *	Reads a message. Should be called after HasMesasge to be sure there is something to read
	 *	@param[out] pCANId 		Message CAN Id
	 *	@param[out] pDataLenght	Read data length
	 *	@param[out] pReadData	Read data
	 *	@return True on success, false otherwise
	 */
	virtual bool ReadImpl(unsigned long& pCANId, unsigned char& pDataLength, unsigned char* pReadData) override
	{
		return this->mCAN->readMsgBuf(&pCANId, 1, &pDataLength, pReadData) == CAN_OK ;
	}

	/**
	 *	Sends data to the given CAN connector
	 *	@param[in] pCANId	CAN Id to use
	 *	@param[in] pLength	Data length 
	 *	@param[in] pData	Data to send
	 *	@return True on success, false otherwise
	 */
	virtual bool SendImpl(unsigned long pCANId, unsigned char pLength, const unsigned char* const pData) override
	{
		return this->mCAN->sendMsgBuf(pCANId, pLength, pData) == CAN_OK ;
	}
	
protected:
	MCP_CAN* mCAN ; /**< CAN library specific object */
} ;