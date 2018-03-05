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
	#define C_CLOSE_SPEED_THRESHOLD	15	/**< Doors close speed threshold in kph */

	/**
	 *	@class CCloseDoors
	 *	Manages the doors automatic lock
	 */
	class CCloseDoors : public MOD::IModule
	{
	public:
		/**
		 *	Constructor
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		CCloseDoors(CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame) : IModule(pCAN, pReadFrame, true, 190)
		{
			// Init members
			this->mShouldClose  = false ;
			this->mShouldOpen   = false ;
		}

	protected:
		/**
		 *	Update method implementation
		 *	@param[in] pTime Current time in ms
		 */
		virtual void UpdateImpl(unsigned long pTime) override
		{
		#ifdef DEBUG_MODULES
			// Print that we are updating the module
			PRINTLN("Updating doors lock module") ;
		#endif
		
			// Needed informations queries succeeded ?
			if (this->mElectricalPower.SendAndUpdate(this->mCAN, this->mReadFrame)
				&& this->mDriverDoorStatus.SendAndUpdate(this->mCAN, this->mReadFrame)
				&& this->mPassengerDoorStatus.SendAndUpdate(this->mCAN, this->mReadFrame)
				&& this->mVehicleSpeed.SendAndUpdate(this->mCAN, this->mReadFrame))
			{
				// Above close speed limit ?
				if (this->mVehicleSpeed.GetCurrentValue() >= C_CLOSE_SPEED_THRESHOLD)
				{
					// We reached close speed limit : we can now open again if needed
					this->mShouldOpen = true ;

					// At least one door opened ?
					if (!this->mDriverDoorStatus.GetCurrentValue() || !this->mPassengerDoorStatus.GetCurrentValue())
					{
						// The system should close the doors ?
						if (this->mShouldClose)
						{
							// Try to close the doors. Succeeded ?
							if (this->mDoorsLock.CloseDoors(this->mCAN))
							{
								// Don't need to close again
								this->mShouldClose = false ;
							}
						}
					}
					else
					{
						// Don't need to close again
						this->mShouldClose = false ;
					}
				}
				// Under close speed limit
				else
				{
					// We are under close speed limit : we can now close again if needed
					this->mShouldClose = true ;

					// At least one door closed ?
					if (this->mDriverDoorStatus.GetCurrentValue() || this->mPassengerDoorStatus.GetCurrentValue())
					{
						// The system should open the doors ?
						if (this->mShouldOpen)
						{
							// Ignition off ?
							if (this->mElectricalPower.GetCurrentValue() == FRS::EElecPowerStatus::EPS_OFF)
							{
								// Open the doors succeeded ?
								if (this->mDoorsLock.OpenDoors(this->mCAN))
								{
									// Don't need to open again
									this->mShouldOpen = false ;
								}
							}
						}
					}
					else
					{
						// Don't need to open again
						this->mShouldOpen = false ;
					}
				}
			}
		}

	protected:
		bool                                mShouldClose ;			/**< Should close status */
		bool                                mShouldOpen ;			/**< Should open status */
		OBD::CVehicleSpeedFrame             mVehicleSpeed ;			/**< Vehicle speed frame */
		FRS::CChangeDoorsLockFrame          mDoorsLock ;			/**< Doors lock management frame */
		FRS::CQueryElecPowerFrame           mElectricalPower ;		/**< Electrical power status frame */
		FRS::CQueryDriverDoorsLockFrame     mDriverDoorStatus ;		/**< Driver's door lock status frame */
		FRS::CQueryPassengerDoorsLockFrame  mPassengerDoorStatus ;	/**< Passenger's door lock status frame */
	} ;
}