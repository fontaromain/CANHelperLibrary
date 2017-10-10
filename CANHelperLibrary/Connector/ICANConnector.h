#pragma once

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
	 *	@param[in] 	pId			Mask Id
	 *	@param[in]	pFrameType	Standard or extended frame
	 *	@param[in] 	pMask		Mask to use
	 *	@return True on success, false otherwise
	 */
	virtual bool SetMask(unsigned char pId, unsigned char pFrameType, unsigned long pMask) = 0 ;
	
	/**
	 *	Sets a specific filter
	 *	@param[in] 	pId			Filter Id
	 *	@param[in]	pFrameType	Standard or extended frame
	 *	@param[in] 	pFilter		Filter to use
	 *	@return True on success, false otherwise
	 */
	virtual bool SetFilter(unsigned char pId, unsigned char pFrameType, unsigned long pFilter) = 0 ;
	
	/**
	 *	Tests if there is something to read or not
	 *	@return True if something, false otherwise
	 */
	virtual bool HasMessages() = 0 ;
	
	/**
	 *	Reads a message. Should be called after HasMesasge to be sure there is something to read
	 *	@param[out] pCANId 		Message CAN Id
	 *	@param[out] pDataLenght	Read data length
	 *	@param[out] pReadData	Read data
	 *	@return True on success, false otherwise
	 */
	virtual bool Read(unsigned long& pCANId, unsigned char& pDataLength, unsigned char* pReadData) = 0 ;

	/**
	 *	Sends data to the given CAN connector
	 *	@param[in] pCANId	CAN Id to use
	 *	@param[in] pLength	Data length 
	 *	@param[in] pData	Data to send
	 *	@return True on success, false otherwise
	 */
	virtual bool Send(unsigned long pCANId, unsigned char pLength, const unsigned char* const pData) = 0 ;
} ;