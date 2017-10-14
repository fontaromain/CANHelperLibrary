#pragma once

// Includes
#include "../AFrame.h"

// Specific class defines
#define STD_FRAME_LENGHT 8 // OBD standard frame bytes length

/**
 *	@class CStdDataFrame
 *	Standard CAN data frame
 *	Used to simplify data creation / usage
 */
class CStdDataFrame : public AFrame
{
public:
	/**
	 *	Constructor
	 */
	CStdDataFrame()
	{
	}

	/**
	 *	Gets data length
	 *	@return Current data length
	 */
	virtual unsigned char GetLength() const override
	{
		return STD_FRAME_LENGHT ;
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
	
protected:
	unsigned long mAddress ; 				/**< Frame address */
	unsigned char mData[STD_FRAME_LENGHT] ;	/**< Data values */
} ;