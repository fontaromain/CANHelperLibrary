//
// Copyright (c) FONTA romain. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

// Includes
#include "../../CGlobals.h"
#include "./CCANDefines.h"

/**
 *	@namespace CAN
 *	Regroup CAN objects
 */
namespace CAN
{
	/**
	 *	@class ACANFrame
	 *	Base CAN frame class
	 */
	class ACANFrame
	{
	public:
		/**
		 *	Destructor
		 */
		virtual ~ACANFrame()
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
		 *	Prints the class content
		 */
		void Print() const
		{
			// Print address first
			PRINT_STR("0x") ;
			PRINTHEX(this->GetAddress()) ;
			PRINT_STR("\t") ;

			// Then each data
			for (int lDataIndex = 0 ; lDataIndex < this->GetLength() ; lDataIndex++)
			{
				PRINT(this->GetData()[lDataIndex] <= 0xF ? " 0x0" : " 0x") ;
				PRINTHEX(this->GetData()[lDataIndex]) ;
			}

			// And go next line
			PRINTLN_STR("") ;
		}
	} ;
}