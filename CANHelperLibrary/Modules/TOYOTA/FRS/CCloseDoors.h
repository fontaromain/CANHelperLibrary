#pragma once

// Includes
#include "../../../Connector/ICANConnector.h"
#include "../../../Frame/OBD/TOYOTA/COBDTOYOTADefines.h"

namespace FRS
{
	/**
	 *	@class CCloseDoors
	 *	Manages the doors automatic lock
	 */
	class CCloseDoors
	{
	public:
		/**
		 *	Constructor
		 */
		CCloseDoors()
		{
			// Init members
			this->mCloseSpeed   = 15 ;
			this->mShouldClose  = false ;
			this->mShouldOpen   = false ;
		}

		/**
		 *	Update method (to call each frame)
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		void Update(CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame)
		{
			// Needed informations queries succeeded ?
			if (this->mElectricalPower.SendAndUpdate(pCAN, pReadFrame)
				&& this->mDriverDoorStatus.SendAndUpdate(pCAN, pReadFrame)
				&& this->mPassengerDoorStatus.SendAndUpdate(pCAN, pReadFrame)
				&& this->mVehicleSpeed.SendAndUpdate(pCAN, pReadFrame))
			{
				// Above close speed limit ?
				if (this->mVehicleSpeed.GetCurrentValue() >= this->mCloseSpeed)
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
							if (this->mDoorsLock.CloseDoors(pCAN))
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
								if (this->mDoorsLock.OpenDoors(pCAN))
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
		unsigned char                       mCloseSpeed ;			/**< Close speed */
		OBD::CVehicleSpeedFrame             mVehicleSpeed ;			/**< Vehicle speed frame */
		FRS::CChangeDoorsLockFrame          mDoorsLock ;			/**< Doors lock management frame */
		FRS::CQueryElecPowerFrame           mElectricalPower ;		/**< Electrical power status frame */
		FRS::CQueryDriverDoorsLockFrame     mDriverDoorStatus ;		/**< Driver's door lock status frame */
		FRS::CQueryPassengerDoorsLockFrame  mPassengerDoorStatus ;	/**< Passenger's door lock status frame */
	} ;
}