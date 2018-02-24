//
// Copyright (c) FONTA romain. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

// Includes
#include "../ICANConnector.h"
#include "mcp_can.h"

/**
 *	@namespace CAN
 *	Regroup CAN objects
 */
namespace CAN
{
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
			this->mCAN = nullptr ;
		}

		/**
		 *	Destructor
		 */
		virtual ~CMCPCANConnector()
		{
			// Clean up if needed
			delete this->mCAN ;
		}

		/**
		 *	Initialize the CAN bus
		 *	@param[in] pSetupFunc Setup function to use
		 *	@return True on success, false otherwise
		 */
		virtual bool Initialize(SetupFiltersAndMasks pSetupFunc = nullptr) override
		{
			// Already a CAN ?
			if (this->mCAN)
			{
				// Cleanup !
				this->Close() ;
			}
			
			// Build a new MCP_CAN using CS pin 10
			this->mCAN = new MCP_CAN(10) ;
			
			// Initialization ok ?
			if (this->mCAN->begin(MCP_STDEXT, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
			{
				// A valid setup functino to use ?
				if (pSetupFunc)
				{
					pSetupFunc(*this) ;
				}
				
				// Back to normal
				this->mCAN->setMode(MCP_NORMAL) ;

				// Succeeded !
				return true ;
			}
			
			// Cleanup !
			this->Close() ;

			// Failed !
			return false ;
		}
		
		/**
		 *	Closes connector
		 *	@return True on success, false otherwise
		 */
		virtual bool Close() override
		{
			// Clean up
			delete this->mCAN ;
			this->mCAN = nullptr ;
			
			// Always succeed
			return true ;
		}

		/**
		 *	Tests whether there is some errors on the connector or not (configuration, etc.)
		 *	@return True if some errors, false otherwise
		 */
		virtual bool HasError() override
		{
			// Error if no CAN available OR if check error invalid
			return !this->mCAN || this->mCAN->checkError() == CAN_CTRLERROR ;
		}
		
		/**
		 *	Gets the current error code if any
		 *	@return Current error code
		 */
		virtual char GetError() override
		{
			return this->mCAN ? this->mCAN->getError() : 0 ;
		}
		
		/**
		 *	Sets a specific mask
		 *	@param[in]  pId			Mask Id
		 *	@param[in]	pFrameType	Standard or extended frame
		 *	@param[in]  pMask		Mask to use
		 *	@return True on success, false otherwise
		 */
		virtual bool SetMask(unsigned char pId, unsigned char pFrameType, unsigned long pMask) override
		{
			return this->mCAN && this->mCAN->init_Mask(pId, pFrameType, pMask) == CAN_OK ;
		}

		/**
		 *	Sets a specific filter
		 *	@param[in]  pId			Filter Id
		 *	@param[in]	pFrameType	Standard or extended frame
		 *	@param[in]  pFilter		Filter to use
		 *	@return True on success, false otherwise
		 */
		virtual bool SetFilter(unsigned char pId, unsigned char pFrameType, unsigned long pFilter) override
		{
			return this->mCAN && this->mCAN->init_Filt(pId, pFrameType, pFilter) == CAN_OK ;
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
		 *	@param[out] pCANId      Message CAN Id
		 *	@param[out] pLength		Read data length
		 *	@param[out] pReadData	Read data
		 *	@return True on success, false otherwise
		 */
		virtual bool ReadImpl(unsigned long& pCANId, unsigned char& pLength, unsigned char* pReadData) override
		{
			return this->mCAN && this->mCAN->readMsgBuf(&pCANId, 1, &pLength, pReadData) == CAN_OK ;
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
			return this->mCAN && this->mCAN->sendMsgBuf(pCANId, pLength, pData) == CAN_OK ;
		}

	protected:
		MCP_CAN* mCAN ;	/**< CAN library specific object */
	} ;
}