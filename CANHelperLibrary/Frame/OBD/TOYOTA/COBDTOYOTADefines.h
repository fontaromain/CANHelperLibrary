//
// Copyright (c) FONTA romain. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

// Includes
#include "../COBDDefines.h"

///////////////////////////////////////////////////////////////////////////////
// OBD Toyota specific defines
#define OBD_MODE_21 0x21	// OBD mode 21 - Toyota specific mode

///////////////////////////////////////////////////////////////////////////////
// Validation functor used for specific TOYOTA OBD messages
#define OBD_TOYOTA_VALIDATION                                       \
	[](const ACANFrame& pFrame, const ACANFrame& pThis) -> bool     \
	{                                                               \
		return pFrame.GetData()[1] > 2                              \
			   && pThis.GetData()[2] + 0x40 == pFrame.GetData()[2]  \
			   && pThis.GetData()[3] == pFrame.GetData()[3] ;       \
	}

/**
 *	@namespace FRS
 *	Specific FRS/BRZ/GT86 OBD messages
 */
namespace FRS
{
	/**
	 *	Setups the masks and filters to be able to use the specific frames defined in this namespace
	 *	@param[in] pCAN Connector to configure
	 */
	void FiltersAndMasksConfiguration(CAN::ICANConnector& pCAN) 
	{
		pCAN.SetFilter(0, 0, 0x07500000) ;
		pCAN.SetFilter(1, 0, 0x07C00000) ;
		pCAN.SetFilter(2, 0, 0x07D00000) ;
		pCAN.SetFilter(3, 0, 0x07E00000) ;
		pCAN.SetFilter(4, 0, 0x07F00000) ;
		pCAN.SetMask(0, 0, 0x07F00000) ;
		pCAN.SetMask(1, 0, 0x07F00000) ;
		pCAN.SetMask(2, 0, 0x07F00000) ;
		pCAN.SetMask(3, 0, 0x07F00000) ;
		pCAN.SetMask(4, 0, 0x07F00000) ;
	}
	/**
	 *	@enum ECombiBtnStatus
	 *	Current combi buttons status
	 */
	enum class ECombiBtnStatus : unsigned char
	{
		S_NONE_PRESSED,		/**< No buttons pressed */
		S_TRIP_PRESSED,		/**< Trip pressed */
		S_DISPLAY_PRESSED,	/**< Display pressed */
		S_BOTH_PRESSED		/**< Both pressed */
	} ;

	/**
	 *	@class CQueryCombiButtonsFrame
	 *	Query the current combi meter buttons status
	 */
	class CQueryCombiButtonsFrame : public OBD::COBDFrame<ECombiBtnStatus>
	{
	public:
		/**
		 *	Constructor
		 */
		CQueryCombiButtonsFrame() : COBDFrame(0x7C0, 0x02, OBD_MODE_21, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C8, OBD_STD_VALIDATION, &OnParse, ECombiBtnStatus::S_NONE_PRESSED)
		{
		}

	protected:
		/**
		 *	Method called once a message is received and passed validation
		 *	@param[in] pFrame Frame to parse
		 *	@return Parsed value
		 */
		static ECombiBtnStatus OnParse(const ACANFrame& pFrame)
		{
			// Extract both statuses
			bool    lDisplayStatus  = ((pFrame.GetData()[5] & 0x04) != 0x0) ;
			bool    lTripStatus     = ((pFrame.GetData()[3] & 0x80) != 0x0) ;

			// Return correct state
			if (lDisplayStatus && lTripStatus)
			{
				return ECombiBtnStatus::S_BOTH_PRESSED ;
			}
			else if (lDisplayStatus && !lTripStatus)
			{
				return ECombiBtnStatus::S_DISPLAY_PRESSED ;
			}
			else if (!lDisplayStatus && lTripStatus)
			{
				return ECombiBtnStatus::S_TRIP_PRESSED ;
			}
			else
			{
				return ECombiBtnStatus::S_NONE_PRESSED ;
			}
		}
	} ;

	/**
	 *	@class CQueryOilTempFrame
	 *	Query the current oil temperature in degrees
	 */
	class CQueryOilTempFrame : public OBD::COBDFrame<char>
	{
	public:
		/**
		 *	Constructor
		 */
		CQueryOilTempFrame() : COBDFrame(0x7E0, 0x02, OBD_MODE_21, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, OBD_BROAD_REPLY_ADDR, OBD_STD_VALIDATION, &OnParse, -40)
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
			return static_cast<char>(pFrame.GetData()[28]) - 40 ;
		}
	} ;

	/**
	 *	@class CChangeDoorsLockFrame
	 *	Query to open or lock the doors
	 */
	class CChangeDoorsLockFrame : public CAN::CSendCANFrame
	{
	public:
		/**
		 *	Constructor
		 */
		CChangeDoorsLockFrame() : CSendCANFrame(0x750, 0x40, 0x05, 0x30, 0x11, 0x00, 0x00, 0x00, 0x00)
		{
		}

		/**
		 *	Tries to close the doors using the given CAN connector
		 *	@param[in] pCAN CAN connector to use
		 *	@return True on success, false otherwise
		 */
		bool CloseDoors(CAN::ICANConnector& pCAN)
		{
			// Change the data setting first
			this->mData[5] = 0x80 ;

			// Perform send
			if (this->SendTo(pCAN))
			{
				// For the system to react
				DELAY(100) ;
				
				// Succeeded
				return true ;
			}
			else
			{
				// Send failed
				return false ;
			}
		}

		/**
		 *	Tries to open the doors using the given CAN connector
		 *	@param[in] pCAN CAN connector to use
		 *	@return True on success, false otherwise
		 */
		bool OpenDoors(CAN::ICANConnector& pCAN)
		{
			// Change the data setting first
			this->mData[5] = 0x40 ;

			// Perform send
			if (this->SendTo(pCAN))
			{
				// For the system to react
				DELAY(100) ;
				
				// Succeeded
				return true ;
			}
			else
			{
				// Send failed
				return false ;
			}
		}
	} ;

	/**
	 *	@class CQueryDriverDoorsLockFrame
	 *	Query current driver's doors lock status
	 */
	class CQueryDriverDoorsLockFrame : public OBD::COBDFrame<bool>
	{
	public:
		/**
		 *	Constructor
		 */
		CQueryDriverDoorsLockFrame() : COBDFrame(0x750, 0x40, 0x02, OBD_MODE_21, 0xA2, 0x00, 0x00, 0x00, 0x00, 0x758, OBD_TOYOTA_VALIDATION, &OnParse, false)
		{
		}

	protected:
		/**
		 *	Method called once a message is received and passed validation
		 *	@param[in] pFrame Frame to parse
		 *	@return Parsed value
		 */
		static bool OnParse(const ACANFrame& pFrame)
		{
			return (pFrame.GetData()[4] & 0x40) == 0x0 ;
		}
	} ;

	/**
	 *	@class CQueryPassengerDoorsLockFrame
	 *	Query current passenger's doors lock status
	 */
	class CQueryPassengerDoorsLockFrame : public OBD::COBDFrame<bool>
	{
	public:
		/**
		 *	Constructor
		 */
		CQueryPassengerDoorsLockFrame() : COBDFrame(0x750, 0x40, 0x02, OBD_MODE_21, 0xA3, 0x00, 0x00, 0x00, 0x00, 0x758, OBD_TOYOTA_VALIDATION, &OnParse, false)
		{
		}

	protected:
		/**
		 *	Method called once a message is received and passed validation
		 *	@param[in] pFrame Frame to parse
		 *	@return Parsed value
		 */
		static bool OnParse(const ACANFrame& pFrame)
		{
			return (pFrame.GetData()[4] & 0x40) == 0x0 ;
		}
	} ;

	/**
	 *	@enum EElecPowerStatus
	 *	Current electrical power status
	 */
	enum class EElecPowerStatus : unsigned char
	{
		EPS_OFF,	/**< Off */
		EPS_ACC,	/**< Accessory on */
		EPS_IGN,	/**< Ignition on */
	} ;

	/**
	 *	@class CQueryElecPowerFrame
	 *	Query the current electrical power status
	 */
	class CQueryElecPowerFrame : public OBD::COBDFrame<EElecPowerStatus>
	{
	public:
		/**
		 *	Constructor
		 */
		CQueryElecPowerFrame() : COBDFrame(0x750, 0x40, 0x02, OBD_MODE_21, 0xA5, 0x00, 0x00, 0x00, 0x00, 0x758, OBD_TOYOTA_VALIDATION, &OnParse, EElecPowerStatus::EPS_OFF)
		{
		}

	protected:
		/**
		 *	Method called once a message is received and passed validation
		 *	@param[in] pFrame Frame to parse
		 *	@return Parsed value
		 */
		static EElecPowerStatus OnParse(const ACANFrame& pFrame)
		{
			// Ignition ?
			if ((pFrame.GetData()[4] & 0xC0) != 0x0)
			{
				return EElecPowerStatus::EPS_IGN ;
			}
			// Accessory ?
			else if ((pFrame.GetData()[4] & 0x40) != 0x0)
			{
				return EElecPowerStatus::EPS_ACC ;
			}
			else
			{
				// Off
				return EElecPowerStatus::EPS_OFF ;
			}
		}
	} ;

	/**
	 *	@enum EGaugePosition
	 *	Gauge position
	 */
	enum class EGaugePosition : unsigned char
	{
		GP_POS_1    = 0x01,	/**< Position 1 */
		GP_POS_2    = 0x02,	/**< Position 2 */
		GP_POS_3    = 0x04,	/**< Position 3 */
		GP_POS_4    = 0x08,	/**< Position 4 */
		GP_POS_5    = 0x10,	/**< Position 5 */
		GP_POS_6    = 0x20,	/**< Position 6 */
		GP_POS_7    = 0x40,	/**< Position 7 */
		GP_POS_8    = 0x80	/**< Position 8 */
	} ;

	/**
	 *	@enum EGaugeType
	 *	Gauge type
	 */
	enum class EGaugeType : unsigned char
	{
		GT_FUEL     = 0x03,	/**< Fuel gauge */
		GT_WATER    = 0x0D,	/**< Water gauge */
	} ;

	/**
	 *	@class CSetGaugePositionFrame
	 *	Sets the given gauge position
	 */
	class CSetGaugePositionFrame : public OBD::COBDFrame<EGaugePosition>
	{
	public:
		/**
		 *	Constructor
		 */
		CSetGaugePositionFrame() : COBDFrame(0x7C0, 0x04, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x758, OBD_TOYOTA_VALIDATION, &OnParse, EGaugePosition::GP_POS_1)
		{
		}

		/**
		 *	Tries to set the gauge position using the given CAN connector
		 *	@param[in] pGauge		Gauge to drive
		 *	@param[in] pPosition	Position to set
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use (to avoid recreating one each time in each OBD frame ...)
		 *	@return True on success, false otherwise
		 */
		bool SetPosition(EGaugeType pGauge, EGaugePosition pPosition, CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame)
		{
			// Change the data setting first
			this->mData[2]  = static_cast<unsigned char>(pGauge) ;
			this->mData[4]  = static_cast<unsigned char>(pPosition) ;

			// Perform send
			return this->SendAndUpdate(pCAN, pReadFrame) ;
		}
	protected:
		/**
		 *	Method called once a message is received and passed validation
		 *	@param[in] pFrame Frame to parse
		 *	@return Parsed value
		 */
		static EGaugePosition OnParse(const ACANFrame& pFrame)
		{
			// The reply only acknowledge that the query is done, so nothing to parse !
			return EGaugePosition::GP_POS_1 ;
		}
	} ;
}