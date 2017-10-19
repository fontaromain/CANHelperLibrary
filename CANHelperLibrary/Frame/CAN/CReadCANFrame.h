#pragma once

// Includes
#include "./ACANFrame.h"
#include "../../Connector/ICANConnector.h"

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
	 *	@param[in] pCAN CAN reader to use
	 *	@return True on success, false otherwise
	 */
	bool ReadFrom(ICANConnector& pCAN)
	{
		// A message available ? And read with success ?
		if (pCAN.HasMessages())
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

						// For each data received in temp buffer skipping first two bytes
						for (int lDataIndex = 2 ; lDataIndex < this->mTempLength ; lDataIndex++)
						{
							// Copy data over correct buffer
							this->mData[lDataIndex - 2] = this->mTempData[lDataIndex] ;
						}
					}

					// We now have to ask CAN bus to send us the remaining data
					{
						// Reuse our temp message data to build a flow control request
						this->mTempData[0] = CAN_FLOW_FRAME_ID << 4 ;	// Flow control identifier

						// And then reset data array
						for (int lDataIndex = 1 ; lDataIndex < CAN_FRAME_LENGHT ; lDataIndex++)
						{
							// Reset !
							this->mTempData[lDataIndex] = CAN_NO_DATA ;
						}

						// Send our flow request using the correct address
						while (!pCAN.Send(this->mAddress - CAN_FLOW_CONTROL_ADDRESS_VALUE, CAN_FRAME_LENGHT, this->mTempData))
						{
							// Wait until send works
						}

						// Wait for at least a reply
						while (!pCAN.HasMessages()) {}
					}

					// And now, parse all the consecutive frames we might receive
					{
						// Store current index position to append correctly next replies
						unsigned short lCurrentIndex = 6 ;	// First frame is 6 data

						// While there are some messages and we haven't read all the data
						while (pCAN.HasMessages() && lCurrentIndex != this->mLength)
						{
							// Read current
							pCAN.Read(this->mTempAddress, this->mTempLength, this->mTempData) ;

							// A consecutive frame ?
							if (CANIsConsecutiveFrame(this->mTempData[0]))
							{
								// For each data received in temp buffer skipping first two bytes
								for (int lDataIndex = 1 ; lDataIndex < this->mTempLength ; lDataIndex++)
								{
									// Copy data over correct buffer
									this->mData[lDataIndex - 1 + lCurrentIndex] = this->mTempData[lDataIndex] ;
								}

								// Increment current index with read data
								lCurrentIndex += 7 ;	// Consecutive frame is 7 data
							}
							// Error !
							else
							{
								// Clean
								this->Clean() ;

								// Leave
								return false ;
							}
						}
					}

					// Seems ok
					return true ;
				}
				// Unknow message
				else
				{
					// Clean
					this->Clean() ;

					// Should not happen but as I'm not a CAN expert ;)
					return false ;
				}
			}
		}

		// Error
		return false ;
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

protected:
	unsigned char   mLength ;							/**< Data length */
	unsigned long   mAddress ;							/**< Frame address */
	unsigned char   mData[CAN_MULTI_FRAME_MAX_LENGHT] ;	/**< Data values  */
	unsigned char   mTempLength ;						/**< Temporary data length */
	unsigned long   mTempAddress ;						/**< Temporary frame address */
	unsigned char   mTempData[CAN_FRAME_LENGHT] ;		/**< Temporary data values */
} ;