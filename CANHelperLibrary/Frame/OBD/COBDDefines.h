#pragma once

///////////////////////////////////////////////////////////////////////////////
// OBD standard defines
#define OBD_BROADCAST_ADDR	0x07DF
#define OBD_REPLY_ADDR		0x07E8
#define OBD_MODE_1 			0x01 	// OBD mode 1 - Show current data

///////////////////////////////////////////////////////////////////////////////
// Validation functor used for standard OBD messages
#define OBD_STANDARD_VALIDATION										\
	[](const ACANFrame& pFrame, const ACANFrame& pThis) -> bool		\
	{																\
		return pFrame.GetData()[0] > 2								\
			   && pThis.GetData()[1] + 0x40 == pFrame.GetData()[1]	\
			   && pThis.GetData()[2] == pFrame.GetData()[2] ;		\
	}

///////////////////////////////////////////////////////////////////////////////
// OBD standard frames	
#define OBD_ENGINE_RPM_FRM(VAR_NAME) 	COBDFrame<unsigned int>		VAR_NAME(OBD_BROADCAST_ADDR, 0x02, OBD_MODE_1, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, OBD_REPLY_ADDR, OBD_STANDARD_VALIDATION, [](const ACANFrame& pF) -> unsigned int { return (unsigned int)(256 * pF.GetData()[3] + pF.GetData()[4]) / 4 ; }, 0)
#define OBD_VEHICLE_SPEED_FRM(VAR_NAME) COBDFrame<unsigned char>	VAR_NAME(OBD_BROADCAST_ADDR, 0x02, OBD_MODE_1, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, OBD_REPLY_ADDR, OBD_STANDARD_VALIDATION, [](const ACANFrame& pF) -> unsigned char { return pF.GetData()[3] ; }, 0)