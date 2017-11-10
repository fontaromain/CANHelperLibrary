//
// Copyright (c) FONTA romain. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

// Includes
#include "../OBD/COBDFrame.h"

///////////////////////////////////////////////////////////////////////////////
// OBD standard defines
#define OBD_BROAD_ADDR          0x07DF
#define OBD_BROAD_REPLY_ADDR    0x07E8
#define OBD_MODE_1              0x01	// OBD mode 1 - Show current data

///////////////////////////////////////////////////////////////////////////////
// Validation functor used for standard OBD messages
#define OBD_STD_VALIDATION                                          \
	[](const ACANFrame& pFrame, const ACANFrame& pThis) -> bool     \
	{                                                               \
		return pFrame.GetData()[0] > 2                              \
			   && pThis.GetData()[1] + 0x40 == pFrame.GetData()[1]  \
			   && pThis.GetData()[2] == pFrame.GetData()[2] ;       \
	}

/**
 *	@namespace OBD
 *  OBD standard frames
 */
namespace OBD
{
	/**
	 *	@class CEngineRPMFrame
	 *	Query the current engine RPM
	 */
	class CEngineRPMFrame : public COBDFrame<unsigned int>
	{
	public:
		/**
		 *	Constructor
		 */
		CEngineRPMFrame() : COBDFrame(OBD_BROAD_ADDR, 0x02, OBD_MODE_1, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, OBD_BROAD_REPLY_ADDR, OBD_STD_VALIDATION, &OnParse, 0)
		{
		}

	protected:
		/**
		 *	Method called once a message is received and passed validation
		 *	@param[in] pFrame Frame to parse
		 *	@return Parsed value
		 */
		static unsigned int OnParse(const ACANFrame& pFrame)
		{
			return (unsigned int)(256 * pFrame.GetData()[3] + pFrame.GetData()[4]) / 4 ;
		}
	} ;

	/**
	 *	@class CVehicleSpeedFrame
	 *	Query the current vehicle speed
	 */
	class CVehicleSpeedFrame : public COBDFrame<unsigned char>
	{
	public:
		/**
		 *	Constructor
		 */
		CVehicleSpeedFrame() : COBDFrame(OBD_BROAD_ADDR, 0x02, OBD_MODE_1, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, OBD_BROAD_REPLY_ADDR, OBD_STD_VALIDATION, &OnParse, 0)
		{
		}

	protected:
		/**
		 *	Method called once a message is received and passed validation
		 *	@param[in] pFrame Frame to parse
		 *	@return Parsed value
		 */
		static unsigned char OnParse(const ACANFrame& pFrame)
		{
			return pFrame.GetData()[3] ;
		}
	} ;

	/**
	 *	@class CVehicleWaterTemperatureFrame
	 *	Query the current vehicle water temperature
	 */
	class CVehicleWaterTemperatureFrame : public COBDFrame<char>
	{
	public:
		/**
		 *	Constructor
		 */
		CVehicleWaterTemperatureFrame() : COBDFrame(OBD_BROAD_ADDR, 0x02, OBD_MODE_1, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, OBD_BROAD_REPLY_ADDR, OBD_STD_VALIDATION, &OnParse, -40)
		{
		}

	protected:
		/**
		 *	Method called once a message is received and passed validation
		 *	@param[in] pFrame Frame to parse
		 *	@return Parsed value
		 */
		static char OnParse(const ACANFrame& pFrame)
		{
			return static_cast<char>(pFrame.GetData()[3]) - 40 ;
		}
	} ;
}