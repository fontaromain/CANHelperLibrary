#pragma once

// Includes
#include "../../IModule.h"
#include "../../../Frame/OBD/TOYOTA/COBDTOYOTADefines.h"

namespace FRS
{
	/**
	 *	@class COilDisplay
	 *	Manages the display of the oil temperature
	 */
	class COilDisplay : public MOD::IModule
	{
	public:
		/**
		 *	Constructor
		 */
		COilDisplay() : IModule(false, 400)
		{
			// Init members
			this->mNormalOilTemperatureThreshold 		= 85 ;
			this->mWarningOilTemperatureThreshold   	= 110 ;
			this->mCriticalOilTemperatureThreshold  	= 120 ;
			this->mCriticalWaterTemperatureThreshold 	= 120 ;
		}

	protected:
		/**
		 *	Event called when the module is enabled
		 *	@param[in] pTime		Current time in ms
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		virtual void OnEnabled(unsigned long pTime, CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame) override
		{
			// Tell user by moving the temp gauge
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, pCAN, pReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_8, pCAN, pReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, pCAN, pReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_8, pCAN, pReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, pCAN, pReadFrame) ;
			DELAY(600) ;
		}
		
		/**
		 *	Event called when the module is disabled
		 *	@param[in] pTime		Current time in ms
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		virtual void OnDisabled(unsigned long pTime, CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame) override 
		{
			// Tell user by moving the temp gauge
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, pCAN, pReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_8, pCAN, pReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, pCAN, pReadFrame) ;
			DELAY(600) ;
		}
		
		/**
		 *	Update method implementation
		 *	@param[in] pTime		Current time in ms
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		virtual void UpdateImpl(unsigned long pTime, CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame) override
		{
		#ifdef DEBUG_MODULES
			// Print that we are updating the module
			PRINTLN("Updating oil display module") ;
		#endif
		
			// Query water / oil temperatures succeeded ?
			if (this->mWaterTemperature.SendAndUpdate(pCAN, pReadFrame) && this->mOilTemperature.SendAndUpdate(pCAN, pReadFrame))
			{				
				// Water temperature is correct ?
				if (this->mWaterTemperature.GetCurrentValue() < mCriticalWaterTemperatureThreshold)
				{
					// Oil too hot ?
					if (this->mOilTemperature.GetCurrentValue() >= this->mCriticalOilTemperatureThreshold)
					{
						this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_8, pCAN, pReadFrame) ;
					}
					// Oil almost too hot ?
					else if (this->mOilTemperature.GetCurrentValue() >= this->mWarningOilTemperatureThreshold)
					{
						this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_7, pCAN, pReadFrame) ;
					}
					// Oil normal ?
					else if (this->mOilTemperature.GetCurrentValue() >= this->mNormalOilTemperatureThreshold)
					{
						// Let the water informations take the control of the gauge
					}
					// Oil cold ?
					else
					{
						this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, pCAN, pReadFrame) ;
					}
				}
				else
				{
					// Water temperature too hot ! Stop displaying oil one, this is an emergency
				}
			}
		}

	protected:
		char								mCriticalWaterTemperatureThreshold ;	/**< Critical water temprature after which oil display is disabled (emergency) */
		char                                mNormalOilTemperatureThreshold ;		/**< Above this value oil is in normal temperature range (in degrees) */
		char                                mWarningOilTemperatureThreshold ;		/**< Above this value oil is in warning temperature range (in degrees) */
		char                                mCriticalOilTemperatureThreshold ;		/**< Above this value oil is in critical temperature range (in degrees) */
		FRS::CSetGaugePositionFrame         mGaugeDriver ;							/**< Frame to drive the cluster gauges */
		FRS::CQueryOilTempFrame             mOilTemperature ;						/**< Query the current oil temperature */
		OBD::CVehicleWaterTemperatureFrame  mWaterTemperature ;						/**< Query the current water temperature */
	} ;
}