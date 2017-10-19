#pragma once

// Includes
#include "../CAN/CSendCANFrame.h"
#include "./COBDDefines.h"

/**
 *	@class CSendCANFrameEx
 *	CAN frame class extended (allows parsing of a reply)
 *	Used to simplify data creation / usage
 */
template<typename TValueType>
class COBDFrame : public CSendCANFrame
{
public:
	typedef TValueType (* ReadFunction)(const ACANFrame& pFrame) ;							/**< Read function declaration */
	typedef bool (* ValidationFunction)(const ACANFrame& pFrame, const ACANFrame& pThis) ;	/**< Validation function declaration */

public:
	/**
	 *	Constructor
	 *	@param[in] pAddress         Frame address
	 *	@param[in] pD0				Data 0
	 *	@param[in] pD1				Data 1
	 *	@param[in] pD2				Data 2
	 *	@param[in] pD3				Data 3
	 *	@param[in] pD4				Data 4
	 *	@param[in] pD5				Data 5
	 *	@param[in] pD6				Data 6
	 *	@param[in] pD7				Data 7
	 *	@param[in] pReplyAddress    Reply address to use
	 *	@param[in] pValidFunction   Validation function to use
	 *	@param[in] pReadFunction    Read function to use
	 *	@param[in] pDefaultValue    Default value to use
	 */
	COBDFrame(unsigned long         pAddress,
			  unsigned char         pD0,
			  unsigned char         pD1,
			  unsigned char         pD2,
			  unsigned char         pD3,
			  unsigned char         pD4,
			  unsigned char         pD5,
			  unsigned char         pD6,
			  unsigned char         pD7,
			  unsigned long         pReplyAddress,
			  ValidationFunction    pValidFunction,
			  ReadFunction          pReadFunction,
			  TValueType            pDefaultValue)
		: CSendCANFrame(pAddress, pD0, pD1, pD2, pD3, pD4, pD5, pD6, pD7)
	{
		// Init members
		this->mReplyAddress     = pReplyAddress ;
		this->mCurrentValue     = pDefaultValue ;
		this->mReadFunction     = pReadFunction ;
		this->mValidFunction    = pValidFunction ;
	}

	/**
	 *	Functor to parse received data
	 *	@param[in] pFrame Frame to use
	 */
	void operator()(const ACANFrame& pFrame)
	{
		// Valid function ? and given frame seems to be what we want ?
		if (this->mReadFunction && this->IsCompatibleReply(pFrame))
		{
			// Parse and get value !
			this->mCurrentValue = this->mReadFunction(pFrame) ;
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
	 *	Gets reply address we are looking for
	 *	@return Current reply address
	 */
	unsigned long GetReplyAddress() const
	{
		return this->mReplyAddress ;
	}

	/**
	 *	Tests if the given frame seems to be a valid reply to the current one
	 *	@param[in] pFrame Frame to test with
	 *	@return True if valid, false otherwise
	 */
	bool IsCompatibleReply(const ACANFrame& pFrame) const
	{
		return this->mValidFunction && this->GetReplyAddress() == pFrame.GetAddress() && this->mValidFunction(pFrame, *this) ;
	}

protected:
	TValueType          mCurrentValue ;		/**< Current value */
	ReadFunction        mReadFunction ;		/**< Read function */
	ValidationFunction  mValidFunction ;	/**< Validation function */
	unsigned long       mReplyAddress ;		/**< The reply address we are looking for */
} ;