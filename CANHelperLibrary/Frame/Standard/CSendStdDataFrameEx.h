#pragma once

// Includes
#include "./CSendStdDataFrame.h"

/**
 *	@class CSendStdDataFrameEx
 *	Standard CAN data frame class extended (allows parsing of a reply)
 *	Used to simplify data creation / usage
 */
template<typename TValueType>
class CSendStdDataFrameEx : public CSendStdDataFrame
{
public:
	typedef TValueType(*ReadFunction)(const AFrame& pData) ;	/**< Read function declaration */

public:
	/**
	 *	Constructor
	 *	@param[in] pAddress 		Frame address
	 *	@param[in] pReplyAddress 	Reply address to use
	 *	@param[in] pD0				Data 0
	 *	@param[in] pD1				Data 1
	 *	@param[in] pD2				Data 2
	 *	@param[in] pD3				Data 3
	 *	@param[in] pD4				Data 4
	 *	@param[in] pD5				Data 5
	 *	@param[in] pD6				Data 6
	 *	@param[in] pD7				Data 7
	 *	@param[in] pReadFunction 	Read function to use
	 *	@param[in] pDefaultValue 	Default value to use
	 */
	CSendStdDataFrameEx(unsigned long pAddress, unsigned long pReplyAddress, unsigned char pD0 = 0x0, unsigned char pD1 = 0x0, unsigned char pD2 = 0x0, unsigned char pD3 = 0x0, unsigned char pD4 = 0x0, unsigned char pD5 = 0x0, unsigned char pD6 = 0x0, unsigned char pD7 = 0x0, ReadFunction pReadFunction = nullptr, TValueType pDefaultValue = 0) 
	 : CSendStdDataFrame(pAddress, pD0, pD1, pD2, pD3, pD4, pD5, pD6, pD7) 
	{
		this->mReplyAddress = pReplyAddress ;
		this->mCurrentValue = pDefaultValue ;
		this->mReadFunction = pReadFunction ;
	}
	
	/**
	 *	Functor to parse received data
	 *	@param[in] pData Data to use
	 *	@return Parsed data
	 */
	void operator()(const AFrame& pData)
	{
		// Valid function ? and given frame seems to be what we want ?
		if (this->mReadFunction && this->IsCompatibleReply(pData))
		{
			// Parse and get value !
			this->mCurrentValue = this->mReadFunction(pData) ;
		}
	}

	/**
	 *	Gets current value
	 *	@return Current value
	 */
	TValueType GetCurrentValue() const
	{
		return this->mCurrentValue ;
	}
	
	/**
	 *	Tests if the given frame seems to be a valid reply to the current one
	 *	@param[in] pFrame Frame to test with
	 *	@return True if valid, false otherwise
	 */
	bool IsCompatibleReply(const AFrame& pFrame)
	{
		return this->mReplyAddress == pFrame.GetAddress()			// The reply has the correct address ? 
			   && pFrame.GetData()[0] > 2							// The data lenght is over two (frame mode + command + some data) ?
			   && this->GetData()[1] + 0x40 == pFrame.GetData()[1]	// The reply is the frame mode + 0x40 ? 
			   && this->GetData()[2] == pFrame.GetData()[2] ;		// And the command id is the same ?
	}
	
protected:
	TValueType		mCurrentValue ;	/**< Current value */
	ReadFunction	mReadFunction ;	/**< Read function */
	unsigned long	mReplyAddress ; /**< The reply address we are looking for */
} ;