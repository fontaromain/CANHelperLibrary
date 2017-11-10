//
// Copyright (c) FONTA romain. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

// Includes
#include "./ACANFrame.h"
#include "../../Connector/ICANConnector.h"

/**
 *	@namespace CAN
 *	Regroup CAN objects
 */
namespace CAN
{
	/**
	 *	@class CSendCANFrame
	 *	A frame used to send CAN data
	 */
	class CSendCANFrame : public ACANFrame
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
		CSendCANFrame(unsigned long pAddress,
					  unsigned char pD0 = 0x0,
					  unsigned char pD1 = 0x0,
					  unsigned char pD2 = 0x0,
					  unsigned char pD3 = 0x0,
					  unsigned char pD4 = 0x0,
					  unsigned char pD5 = 0x0,
					  unsigned char pD6 = 0x0,
					  unsigned char pD7 = 0x0)
		{
			// Initialize address and length
			this->mAddress  = pAddress ;
			this->mLength   = CAN_FRAME_LENGHT ;

			// Initialize data
			this->mData[0]  = pD0 ;
			this->mData[1]  = pD1 ;
			this->mData[2]  = pD2 ;
			this->mData[3]  = pD3 ;
			this->mData[4]  = pD4 ;
			this->mData[5]  = pD5 ;
			this->mData[6]  = pD6 ;
			this->mData[7]  = pD7 ;
		}

		/**
		 *	Gets data length
		 *	@return Current data length
		 */
		virtual unsigned char GetLength() const override
		{
			return this->mLength ;
		}

		/**
		 *	Gets frame address
		 *	@return Current frame address
		 */
		virtual unsigned long GetAddress() const override
		{
			return this->mAddress ;
		}

		/**
		 *	Gets data
		 *	@return Current data
		 */
		virtual const unsigned char* const GetData() const override
		{
			return this->mData ;
		}

		/**
		 *	Sends data to the given CAN connector
		 *	@param[in] pCAN CAN connector to use
		 *	@return True on success, false otherwise
		 */
		bool SendTo(ICANConnector& pCAN) const
		{
			return pCAN.WaitSend(this->GetAddress(), this->GetLength(), this->GetData()) ;
		}

	protected:
		unsigned char   mLength ;					/**< Data length */
		unsigned long   mAddress ;					/**< Frame address */
		unsigned char   mData[CAN_FRAME_LENGHT] ;	/**< Data values */
	} ;
}