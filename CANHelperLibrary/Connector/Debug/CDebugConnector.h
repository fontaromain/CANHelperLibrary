//
// Copyright (c) FONTA romain. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

// Includes
#include "../ICANConnector.h"
#include "../../Frame/OBD/COBDDefines.h"

/**
 *	@namespace DEBUG
 *	Regroup DEBUG objects
 */
namespace DEBUG
{
	/**
	 *	@class CDebugConnector
	 *	A debug connector if you need to do some tests
	 */
	class CDebugConnector : public CAN::ICANConnector
	{
	public:
		/**
		 *	Constructor
		 */
		CDebugConnector()
		{
			this->mMessageIndex = 0 ;
			this->mHasMessages  = false ;
		}

		/**
		 *	Destructor
		 */
		virtual ~CDebugConnector()
		{
		}
		
		/**
		 *	Initialize the CAN bus
		 *	@param[in] pSetupFunc Setup function to use
		 *	@return True on success, false otherwise
		 */
		virtual bool Initialize(SetupFiltersAndMasks pSetupFunc = nullptr) override
		{
			return true ;
		}

		/**
		 *	Closes connector
		 *	@return True on success, false otherwise
		 */
		virtual bool Close() override
		{
			return true ;
		}

		/**
		 *	Tests whether there is some errors on the connector or not (configuration, etc.)
		 *	@return True if some errors, false otherwise
		 */
		virtual bool HasError() override
		{
			return false ;
		}
		
		/**
		 *	Gets the current error code if any
		 *	@return Current error code
		 */
		virtual char GetError() override
		{
			return 0 ;
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
			return true ;
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
			return true ;
		}

		/**
		 *	Tests if there is something to read or not
		 *	@return True if something, false otherwise
		 */
		virtual bool HasMessages() override
		{
			return this->mHasMessages ;
		}

		/**
		 *	Reads a frame implementation
		 *	@param[out] pCANId      Message CAN Id
		 *	@param[out] pLength		Read data length
		 *	@param[out] pReadData	Read data
		 *	@return True on success, false otherwise
		 */
		virtual bool ReadImpl(unsigned long& pCANId, unsigned char& pLength, unsigned char* pReadData) override
		{
			if (this->mMessageIndex == 0)
			{
				// Define data
				pCANId          = 0x7E8 ;
				pLength         = 0x08 ;
				pReadData[0]    = 0x10 ;
				pReadData[1]    = 0x1F ;
				pReadData[2]    = 0x61 ;
				pReadData[3]    = 0x01 ;
				pReadData[4]    = 0x51 ;
				pReadData[5]    = 0x00 ;
				pReadData[6]    = 0x35 ;
				pReadData[7]    = 0x01 ;

				// Go next
				this->mMessageIndex++ ;
			}
			else if (this->mMessageIndex == 1)
			{
				// Define data
				pCANId          = 0x7E8 ;
				pLength         = 0x08 ;
				pReadData[0]    = 0x21 ;
				pReadData[1]    = 0x36 ;
				pReadData[2]    = 0x25 ;
				pReadData[3]    = 0x4D ;
				pReadData[4]    = 0x4A ;
				pReadData[5]    = 0x62 ;
				pReadData[6]    = 0x62 ;
				pReadData[7]    = 0x0B ;

				// Go next
				this->mMessageIndex++ ;
			}
			else if (this->mMessageIndex == 2)
			{
				// Define data
				pCANId          = 0x7E8 ;
				pLength         = 0x08 ;
				pReadData[0]    = 0x22 ;
				pReadData[1]    = 0xC3 ;
				pReadData[2]    = 0x00 ;
				pReadData[3]    = 0x00 ;
				pReadData[4]    = 0x8E ;
				pReadData[5]    = 0x07 ;
				pReadData[6]    = 0x25 ;
				pReadData[7]    = 0x50 ;

				// Go next
				this->mMessageIndex++ ;
			}
			else if (this->mMessageIndex == 3)
			{
				// Define data
				pCANId          = 0x7E8 ;
				pLength         = 0x08 ;
				pReadData[0]    = 0x23 ;
				pReadData[1]    = 0x22 ;
				pReadData[2]    = 0x22 ;
				pReadData[3]    = 0x00 ;
				pReadData[4]    = 0x08 ;
				pReadData[5]    = 0x00 ;
				pReadData[6]    = 0x8E ;
				pReadData[7]    = 0x01 ;

				// Go next
				this->mMessageIndex++ ;
			}
			else if (this->mMessageIndex == 4)
			{
				// Define data
				pCANId          = 0x7E8 ;
				pLength         = 0x08 ;
				pReadData[0]    = 0x24 ;
				pReadData[1]    = 0x08 ;
				pReadData[2]    = 0x36 ;
				pReadData[3]    = 0x8C ;
				pReadData[4]    = 0x79 ;
				pReadData[5]    = 0x00 ;
				pReadData[6]    = 0x00 ;
				pReadData[7]    = 0x00 ;

				// Loop over
				this->mMessageIndex = 0 ;
			}

			// No more messages
			this->mHasMessages = false ;

			// Succeeded
			return true ;
		}

		/**
		 *	Sends a frame implementation
		 *	@param[in] pCANId	CAN Id to use
		 *	@param[in] pLength	Data length
		 *	@param[in] pData	Data to send
		 *	@return True on success, false otherwise
		 */
		virtual bool SendImpl(unsigned long pCANId, unsigned char pLength, const unsigned char* const pData) override
		{
			// New messages following this send
			this->mHasMessages = true ;

			// Ok !
			return false ;
		}

	protected:
		bool            mHasMessages ;	/**< Messages status */
		unsigned int    mMessageIndex ;	/**< Current message index */
	} ;
}