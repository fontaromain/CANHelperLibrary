//
// Copyright (c) FONTA romain. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

///////////////////////////////////////////////////////////////////////////////
// Standard CAN
#define CAN_FRAME_LENGHT    8	// CAN frame length in bytes
#define CAN_NO_DATA         0x0	// No data
#define CAN_TIMEOUT         500	// In ms

///////////////////////////////////////////////////////////////////////////////
// ISO 15765-2

// Frame types
#define CAN_SINGLE_FRAME_ID         0x0	// Single frame ID
#define CAN_FIRST_FRAME_ID          0x1	// First frame ID
#define CAN_CONSECUTIVE_FRAME_ID    0x2	// Consecutive frame ID
#define CAN_FLOW_FRAME_ID           0x3	// Flow frame ID

// Useful defines
#define CAN_MULTI_FRAME_MAX_LENGHT      128		// CAN multiframe max length in bytes (4096 is the max length defined by the ISO, use your own according to your applications)
#define CAN_FLOW_CONTROL_ADDRESS_VALUE  0x08	// CAN flow control address to substract from the first frame message address

/**
 *	Tells whether the frame is a single frame or not
 *	@return Current status
 */
bool CANIsSingleFrame(const unsigned char& pValue)
{
	return ((pValue & 0xF0) >> 4) == CAN_SINGLE_FRAME_ID ;
}

/**
 *	Tells whether the frame is a first frame or not
 *	@return Current status
 */
bool CANIsFirstFrame(const unsigned char& pValue)
{
	return ((pValue & 0xF0) >> 4) == CAN_FIRST_FRAME_ID ;
}

/**
 *	Tells whether the frame is a consecutive frame or not
 *	@return Current status
 */
bool CANIsConsecutiveFrame(const unsigned char& pValue)
{
	return ((pValue & 0xF0) >> 4) == CAN_CONSECUTIVE_FRAME_ID ;
}