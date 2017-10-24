#pragma once

// Includes
#include "../COBDDefines.h"

///////////////////////////////////////////////////////////////////////////////
// OBD Toyota specific defines
#define OBD_MODE_21 0x21 // OBD mode 21 - Toyota specific mode

///////////////////////////////////////////////////////////////////////////////
// Specific FRS/BRZ/GT86 OBD messages
#define OBD_TOYOTA_FRS_OIL_TEMP(VAR_NAME)		COBDFrame<char> VAR_NAME(0x7E0, 0x02, OBD_MODE_21, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, OBD_REPLY_ADDR, OBD_STANDARD_VALIDATION, [](const ACANFrame& pF) -> char { return (char)(pF.GetData()[28]) - 40 ; }, -40) ;
#define OBD_TOYOTA_FRS_LOCK_DOORS(VAR_NAME)		CSendCANFrame	VAR_NAME(0x750, 0x40, 0x05, 0x30, 0x11, 0x00, 0x80, 0x00, 0x00) ;
#define OBD_TOYOTA_FRS_UNLOCK_DOORS(VAR_NAME)	CSendCANFrame	VAR_NAME(0x750, 0x40, 0x05, 0x30, 0x11, 0x00, 0x40, 0x00, 0x00) ;