#pragma once 

// OBD standard defines
#define BROADCAST_ADDR	0x07DF
#define REPLY_ADDR		0x07E8
#define OBD_MODE_1 		0x01 	// OBD mode 1 - Show current data

// OBD standard frames
#define ENGINE_RPM_FRM(VAR_NAME) 	CSendStdDataFrameEx<unsigned int>	VAR_NAME(BROADCAST_ADDR, REPLY_ADDR, 0x02, OBD_MODE_1, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, [](const AFrame& pData) -> unsigned int { return (unsigned int)(256 * pData.GetData()[3] + pData.GetData()[4]) / 4 ; })
#define VEHICLE_SPEED_FRM(VAR_NAME) CSendStdDataFrameEx<unsigned char>	VAR_NAME(BROADCAST_ADDR, REPLY_ADDR, 0x02, OBD_MODE_1, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, [](const AFrame& pData) -> unsigned char { return pData.GetData()[3] ; })


// Debug defines
#define DEBUG_SEND 		0
#define DEBUG_RECEIVE	0