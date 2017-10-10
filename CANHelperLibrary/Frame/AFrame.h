#pragma once 

// Includes
#include "../CGlobals.h"

/**
 *	@class AFrame
 *	Base data frame class
 */
class AFrame
{
public:
	/**
	 *	Destructor
	 */
	virtual ~AFrame()
	{
	}
	
	/**
	 *	Gets data length
	 *	@return Current data length
	 */
	virtual unsigned char GetLength() const = 0 ;

	/**
	 *	Gets frame address
	 *	@return Current frame address
	 */
	virtual unsigned long GetAddress() const = 0 ;

	/**
	 *	Gets data
	 *	@return Current data
	 */
	virtual const unsigned char* const GetData() const = 0 ;
	
	/**
	 *	Prints the class content to the serial output
	 */
	void Print() const
	{
		Serial.print("0x") ;
		Serial.print(this->GetAddress(), HEX) ;
		Serial.print("\t") ;
		
		for (int lDataIndex = 0 ; lDataIndex < this->GetLength() ; lDataIndex++)
		{
			Serial.print(this->GetData()[lDataIndex] <= 0xF ? " 0x0" : " 0x") ;			
			Serial.print(this->GetData()[lDataIndex], HEX) ;
		}
		
		Serial.println() ;
	}
} ;