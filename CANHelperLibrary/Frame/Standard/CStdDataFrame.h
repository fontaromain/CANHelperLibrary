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
	
	/**
	 *	Tests if the given frame seems to be the same as the current one (e.g. it shares the same "command")
	 *	@param[in] pFrame Frame to test with
	 *	@return True if similar, false otherwise
	 */
	bool IsSameAs(const CStdDataFrame& pFrame)
	{
		return this->GetData()[2] == pFrame.GetData()[2] ;
	}
	
protected:
	unsigned long mAddress ; 				/**< Frame address */
	unsigned char mData[STD_FRAME_LENGHT] ;	/**< Data values */
} ;