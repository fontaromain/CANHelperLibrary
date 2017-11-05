#pragma once

// Includes
#include "../../../Connector/ICANConnector.h"
#include "../../../Frame/OBD/TOYOTA/COBDTOYOTADefines.h"

namespace FRS
{
	/**
	 *	@class COilDisplay
	 *	Manages the display of the oil temperature
	 */
	class COilDisplay
	{
	public:
		/**
		 *	Constructor
		 */
		COilDisplay()
		{
			// Init members
			this->mActivated                        	= false ;
			this->mNormalOilTemperatureThreshold 		= 85 ;
			this->mWarningOilTemperatureThreshold   	= 110 ;
			this->mCriticalOilTemperatureThreshold  	= 120 ;
			this->mCriticalWaterTemperatureThreshold 	= 120 ;
		}

		/**
		 *	Toggle module activation
		 *	@param[in] pCAN CAN connector to use
		 */
		void ToggleActivation(CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame)
		{
			// Activate or desactivate the oil show
			this->mActivated = !this->mActivated ;

			// And tell user by moving the temp gauge from cold to hot to cold again
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, pCAN, pReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_8, pCAN, pReadFrame) ;
			DELAY(600) ;
			this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, pCAN, pReadFrame) ;
			DELAY(600) ;

			// Activated ?
			if (this->mActivated)
			{
				// Do more gauges moves
				this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_8, pCAN, pReadFrame) ;
				DELAY(600) ;
				this->mGaugeDriver.SetPosition(FRS::EGaugeType::GT_WATER, FRS::EGaugePosition::GP_POS_1, pCAN, pReadFrame) ;
				DELAY(600) ;
			}
		}

		/**
		 *	Update method (to call each frame)
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		void Update(CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame)
		{
			// The users wants the oil temperature ?
			if (this->mActivated)
			{
				// Query water temprature
				this->mWaterTemperature.SendAndUpdate(pCAN, pReadFrame) ;
				
				// Water temperature is correct ?
				if (this->mWaterTemperature.GetCurrentValue() < mCriticalWaterTemperatureThreshold)
				{
					// Query oil temperature
					this->mOilTemperature.SendAndUpdate(pCAN, pReadFrame) ;

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
		bool                                mActivated ;							/**< If the module is activated or not */
		FRS::CSetGaugePositionFrame         mGaugeDriver ;							/**< Frame to drive the cluster gauges */
		FRS::CQueryOilTempFrame             mOilTemperature ;						/**< Query the current oil temperature */
		OBD::CVehicleWaterTemperatureFrame  mWaterTemperature ;						/**< Query the current water temperature */
	} ;
}