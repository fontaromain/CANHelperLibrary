//
// Copyright (c) FONTA romain. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

// Includes
#include "../Connector/ICANConnector.h"
#include "../Frame/CAN/CReadCANFrame.h"

namespace MOD
{
	/**
	 *	@class IModule
	 *	Base class for modules
	 */
	class IModule
	{
	public:
		/**
		 *	Constructor
		 *	@param[in] pCAN					CAN connector to use
		 *	@param[in] pReadFrame			Read frame to use
		 *	@param[in] pEnabledByDefault	If the module is enabled by default
		 *	@param[in] pUpdateRate			Wanted update rate
		 */
		IModule(CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame, bool pEnabledByDefault, unsigned long pUpdateRate)
		 : mCAN(pCAN), mReadFrame(pReadFrame)
		{
			// Init members
			this->mLastUpdateTime	= 0 ;
			this->mUpdateRate 		= pUpdateRate ;
			this->mEnabled  		= pEnabledByDefault ;
		}
		
		/**
		 *	Enable the module
		 *	@param[in] pTime Current time in ms
		 */
		void Enable(unsigned long pTime)
		{
			// Now enabled
			this->mEnabled = true ;
			
			// Call event
			this->OnEnabled(pTime) ;
		}
		
		/**
		 *	Disable the module
		 *	@param[in] pTime Current time in ms
		 */
		void Disable(unsigned long pTime)
		{
			// Now disabled
			this->mEnabled = false ;
			
			// Call event
			this->OnDisabled(pTime) ;
		}

		/**
		 *	Update method (to call each frame)
		 *	@param[in] pTime Current time in ms
		 */
		void Update(unsigned long pTime)
		{
			// Method called even if not enabled
			this->OnPreUpdate(pTime) ;
			
			// Enabled ?
			if (this->IsEnabled())
			{
				// Update rate reached ?
				if ((unsigned long)(pTime - this->mLastUpdateTime) >= this->GetUpdateRate())
				{
					// Now updated
					this->mLastUpdateTime = pTime ;
					
					// Call implementation
					this->UpdateImpl(pTime) ;
				}
			}
		}
		
		/**
		 *	Tells whether the module is enabled or not
		 *	@return Current status
		 */
		inline bool IsEnabled() const
		{
			return this->mEnabled ;
		}
		
		/**
		 *	Gets current update rate
		 *	@return Current update rate
		 */
		inline unsigned long GetUpdateRate() const
		{
			return this->mUpdateRate ;
		}
		
	protected:
		/**
		 *	Event called when the module is enabled
		 *	@param[in] pTime Current time in ms
		 */
		virtual void OnEnabled(unsigned long pTime) 
		{
		}
		
		/**
		 *	Event called when the module is disabled
		 *	@param[in] pTime Current time in ms
		 */
		virtual void OnDisabled(unsigned long pTime) 
		{
		}
	
		/**
		 *	Pre-update method. Called even if not enabled
		 *	@param[in] pTime Current time in ms
		 */
		virtual void OnPreUpdate(unsigned long pTime) 
		{
		}
	
		/**
		 *	Update method implementation
		 *	@param[in] pTime Current time in ms
		 */
		virtual void UpdateImpl(unsigned long pTime) = 0 ;

	protected:
		CAN::ICANConnector& mCAN ;			/**< Current connector */
		CAN::CReadCANFrame& mReadFrame ;	/**< Current read frame */
		
	private:
		bool 			mEnabled ;			/**< If the module is enabled or not */
		unsigned long	mUpdateRate ;		/**< Current update rate */
		unsigned long	mLastUpdateTime ;	/**< Last update time */
	} ;
}