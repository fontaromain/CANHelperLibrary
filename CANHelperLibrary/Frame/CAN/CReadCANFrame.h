//
// Copyright (c) FONTA romain. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

// Includes
#include "./ACANFrame.h"
#include "../../Connector/ICANConnector.h"

#ifdef DEBUG_CAN
	// Class debug defines
	#define CREADCANFRAME_DEBUG_RECEIVE
#endif

/**
 *	@namespace CAN
 *	Regroup CAN objects
 */
namespace CAN
{
	/**
	 *	@class CReadCANFrame
	 *	A frame used to read CAN data
	 */
	class CReadCANFrame : public ACANFrame
	{
	public:
		/**
		 *	Constructor
		 */
		CReadCANFrame()
		{
			// Init members
			this->Clean() ;
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
		 *	Reads data from the given reader
		 *	@param[in] pCAN CAN connector to use
		 *	@return True on success, false otherwise
		 */
		bool ReadFrom(ICANConnector& pCAN)
		{
			// A message available ?
			if (pCAN.WaitHasMessages())
			{
				// Read succeed ?
				if (pCAN.Read(this->mTempAddress, this->mTempLength, this->mTempData))
				{
					// Did we just read a single frame ?
					if (CANIsSingleFrame(this->mTempData[0]))
					{
						// For each data received in temp buffer
						for (int lDataIndex = 0 ; lDataIndex < this->mTempLength ; lDataIndex++)
						{
							// Copy data over correct buffer
							this->mData[lDataIndex] = this->mTempData[lDataIndex] ;
						}

						// And get correct length and address
						this->mLength   = this->mTempLength ;
						this->mAddress  = this->mTempAddress ;

						// Nothing more to do, it's perfect !
						return true ;
					}
					// A first frame ?
					else if (CANIsFirstFrame(this->mTempData[0]))
					{
						// Extract data from the first frame
						{
							// Extract the lenght and store the address
							this->mLength   = (((unsigned short)(this->mTempData[0] & 0x0F)) << 8) + this->mTempData[1] ;
							this->mAddress  = this->mTempAddress ;

							// For each data received in temp buffer skipping first byte (first frame ID)
							for (int lDataIndex = 1 ; lDataIndex < this->mTempLength ; lDataIndex++)
							{
								// Copy data over correct buffer
								this->mData[lDataIndex - 1] = this->mTempData[lDataIndex] ;
							}
						}

						// We now have to ask CAN bus to send us the remaining data
						// Store current index position to append correctly next replies
						unsigned short lCurrentIndex = 4 ;	// First frame is 4 data (8 - 2 (First frame / lenght ID) - 2 (Mode and Pid))

						// While we haven't read all the data
						while (lCurrentIndex < this->mLength)
						{
							// Make and send flow control succeeded ?
							if (this->MakeAndSendFlowControlTempFrame(this->mAddress, pCAN))
							{
								// Wait for at least a reply
								if (pCAN.WaitHasMessages())
								{
									// And now, parse the consecutive frames we might have received
									pCAN.Read(this->mTempAddress, this->mTempLength, this->mTempData) ;

									// A consecutive frame ?
									if (CANIsConsecutiveFrame(this->mTempData[0]))
									{
										// For each data received in temp buffer skipping first byte (consecutive frame ID)
										for (int lDataIndex = 1 ; lDataIndex < this->mTempLength ; lDataIndex++)
										{
											// Copy data over correct buffer
											this->mData[lDataIndex - 1 + lCurrentIndex] = this->mTempData[lDataIndex] ;
										}

										// Increment current index with read data
										lCurrentIndex += 7 ;	// Consecutive frame is 7 data (8 - consecutive frame ID)
									}
									// Error : not a consecutive frame !
									else
									{
										// Clean
										this->Clean() ;

										// Leave
										return false ;
									}
								}
								// Reply timeout
								else
								{
									return false ;
								}
							}
							// Send timeout
							else
							{
								return false ;
							}
						}
						#ifdef CREADCANFRAME_DEBUG_RECEIVE
							// Print debug
							this->Print() ;
						#endif

						// Seems ok
						return true ;
					}
					// Unknow message
					else
					{
						// For each data received in temp buffer
						for (int lDataIndex = 0 ; lDataIndex < this->mTempLength ; lDataIndex++)
						{
							// Copy data over correct buffer
							this->mData[lDataIndex] = this->mTempData[lDataIndex] ;
						}

						// And get correct length and address
						this->mLength   = this->mTempLength ;
						this->mAddress  = this->mTempAddress ;

						#ifdef CREADCANFRAME_DEBUG_RECEIVE
							// Print debug
							PRINT_STR("Received a specific frame type which is not standard :") ;
							PRINTLNHEX(this->mData[0] & 0xF0) ;
						#endif

						// Specific frame type which is non standard
						return true ;
					}
				}
				else
				{
					// Read failed
					return false ;
				}
			}
			else
			{
				// No messages
				return false ;
			}
		}

	protected:
		/**
		 *	Cleans the object
		 */
		void Clean()
		{
			this->mLength   = 0 ;
			this->mAddress  = 0 ;
		}

		/**
		 *	Makes a flow control frame and sends it
		 *	@param[in] pOriginalAddress First frame original address
		 *	@param[in] pCAN				CAN connector to use
		 *	@return True on sucess, false otherwise
		 */
		bool MakeAndSendFlowControlTempFrame(unsigned long pOriginalAddress, ICANConnector& pCAN)
		{
			// Define address and length
			this->mTempAddress  = pOriginalAddress - CAN_FLOW_CONTROL_ADDRESS_VALUE ;
			this->mTempLength   = CAN_FRAME_LENGHT ;

			// Define data
			this->mTempData[0]  = CAN_FLOW_FRAME_ID << 4 ;	// Flow control identifier
			this->mTempData[1]  = 0x01 ;					// We want the CAN bus to sends us one consecutive frame only
			this->mTempData[2]  = CAN_NO_DATA ;
			this->mTempData[3]  = CAN_NO_DATA ;
			this->mTempData[4]  = CAN_NO_DATA ;
			this->mTempData[5]  = CAN_NO_DATA ;
			this->mTempData[6]  = CAN_NO_DATA ;
			this->mTempData[7]  = CAN_NO_DATA ;

			// Send our flow request using the correct address and wait until send works or time out
			return pCAN.WaitSend(this->mAddress - CAN_FLOW_CONTROL_ADDRESS_VALUE, CAN_FRAME_LENGHT, this->mTempData) ;
		}

	protected:
		unsigned char   mLength ;							/**< Data length */
		unsigned long   mAddress ;							/**< Frame address */
		unsigned char   mData[CAN_MULTI_FRAME_MAX_LENGHT] ;	/**< Data values  */
		unsigned char   mTempLength ;						/**< Temporary data length */
		unsigned long   mTempAddress ;						/**< Temporary frame address */
		unsigned char   mTempData[CAN_FRAME_LENGHT] ;		/**< Temporary data values */
	} ;
}