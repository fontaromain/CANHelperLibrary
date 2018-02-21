//
// Copyright (c) FONTA romain. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

// Includes
#include "../../IModule.h"
#include "../../../Frame/OBD/TOYOTA/COBDTOYOTADefines.h"

namespace FRS
{
	// Threshold values
	#define C_CRITICAL_WATER_TEMPERATURE_THRESHOLD	120		/**< Critical water temprature after which oil display is disabled (emergency) */
	#define C_NORMAL_OIL_TEMPERATURE_THRESHOLD 		85		/**< Above this value oil is in normal temperature range (in degrees) */
	#define C_WARNING_OIL_TEMPERATURE_THRESHOLD 	110		/**< Above this value oil is in warning temperature range (in degrees) */
	#define C_CRITICAL_OIL_TEMPERATURE_THRESHOLD 	120		/**< Above this value oil is in critical temperature range (in degrees) */
	#define C_COMBI_DISPLAY_PUSH_DURATION			5000	/**< Duration in ms to press the display combi button to enable / disable the module */
	
	/**
	 *	@class COilDisplay
	 *	Manages the display of the oil temperature
	 */
	class COilDisplay : public MOD::IModule
	{
	public:
		/**
		 *	Constructor
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		COilDisplay(CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame) : IModule(pCAN, pReadFrame, true, 400)
		{
			// Init members
			this->mCombiDisplayButtonPressStart = 0 ;
		}
		
		/**
		 *	Gets current oil temperature
		 *	@return Current oil temperature
		 */
		char GetCurrentOilTemperature() const
		{
			return this->mOilTemperature.GetCurrentValue() ;
		}

	protected:
		/**
		 *	Event called when the module is enabled
		 *	@param[in] pTime Current time in ms
		 */
		virtual void OnEnabled(unsigned long pTime) override
		{
			// Tell user by moving the temp gauge
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, this->mCAN, this->mReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_8, this->mCAN, this->mReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, this->mCAN, this->mReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_8, this->mCAN, this->mReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, this->mCAN, this->mReadFrame) ;
			DELAY(600) ;
		}
		
		/**
		 *	Event called when the module is disabled
		 *	@param[in] pTime Current time in ms
		 */
		virtual void OnDisabled(unsigned long pTime) override 
		{
			// Tell user by moving the temp gauge
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, this->mCAN, this->mReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_8, this->mCAN, this->mReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, this->mCAN, this->mReadFrame) ;
			DELAY(600) ;
		}
		
		/**
		 *	Pre-update method. Called even if not enabled
		 *	@param[in] pTime Current time in ms
		 */
		virtual void OnPreUpdate(unsigned long pTime) override
		{
			// Look for activation / desactivation of the oil temperature display
			if (this->mCombiButtons.SendAndUpdate(this->mCAN, this->mReadFrame))
			{
				// The display button is pressed ?
				if (this->mCombiButtons.GetCurrentValue() == FRS::ECombiBtnStatus::S_DISPLAY_PRESSED)
				{
					// Check if elapsed time is enough to activate the display: enough ?
					if ((unsigned long)(pTime - this->mCombiDisplayButtonPressStart) > C_COMBI_DISPLAY_PUSH_DURATION)
					{
						// Activate or desactivate the module
						if (this->IsEnabled())
						{
							this->Disable(pTime) ;
						}
						else
						{
							this->Enable(pTime) ;
						}

						// Reset time counter
						this->mCombiDisplayButtonPressStart = pTime ;
					}
				}
				else
				{
					// Reset time counter
					this->mCombiDisplayButtonPressStart = pTime ;
				}
			}
			else
			{
				// Reset time counter
				this->mCombiDisplayButtonPressStart = pTime ;
			}
		}
	
		/**
		 *	Update method implementation
		 *	@param[in] pTime Current time in ms
		 */
		virtual void UpdateImpl(unsigned long pTime) override
		{
		#ifdef DEBUG_MODULES
			// Print that we are updating the module
			PRINTLN("Updating oil display module") ;
		#endif
		
			// Query water / oil temperatures succeeded ?
			if (this->mWaterTemperature.SendAndUpdate(this->mCAN, this->mReadFrame) && this->mOilTemperature.SendAndUpdate(this->mCAN, this->mReadFrame))
			{				
				// Water temperature is correct ?
				if (this->mWaterTemperature.GetCurrentValue() < C_CRITICAL_WATER_TEMPERATURE_THRESHOLD)
				{
					// Oil too hot ?
					if (this->mOilTemperature.GetCurrentValue() >= C_CRITICAL_OIL_TEMPERATURE_THRESHOLD)
					{
						this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_8, this->mCAN, this->mReadFrame) ;
					}
					// Oil almost too hot ?
					else if (this->mOilTemperature.GetCurrentValue() >= C_WARNING_OIL_TEMPERATURE_THRESHOLD)
					{
						this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_7, this->mCAN, this->mReadFrame) ;
					}
					// Oil normal ?
					else if (this->mOilTemperature.GetCurrentValue() >= C_NORMAL_OIL_TEMPERATURE_THRESHOLD)
					{
						// Let the water informations take the control of the gauge
					}
					// Oil cold ?
					else
					{
						this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, this->mCAN, this->mReadFrame) ;
					}
				}
				else
				{
					// Water temperature too hot ! Stop displaying oil one, this is an emergency
				}
			}
		}

	protected:
		unsigned long                   	mCombiDisplayButtonPressStart ;	/**< Time when the user started to press the cluster display button */
		FRS::CSetGaugePositionFrame         mGaugeDriver ;					/**< Frame to drive the cluster gauges */
		FRS::CQueryOilTempFrame             mOilTemperature ;				/**< Query the current oil temperature */
		OBD::CVehicleWaterTemperatureFrame  mWaterTemperature ;				/**< Query the current water temperature */
		FRS::CQueryCombiButtonsFrame		mCombiButtons ;					/**< Frame to query combi buttons status */
	} ;
}