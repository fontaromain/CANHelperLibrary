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
		 *	@param[in] pEnabledByDefault	If the module is enabled by default
		 *	@param[in] pUpdateRate			Wanted update rate
		 */
		IModule(bool pEnabledByDefault, unsigned long pUpdateRate)
		{
			// Init members
			this->mLastUpdateTime	= 0 ;
			this->mUpdateRate 		= pUpdateRate ;
			this->mEnabled  		= pEnabledByDefault ;
		}
		
		/**
		 *	Enable the module
		 *	@param[in] pTime		Current time in ms
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		void Enable(unsigned long pTime, CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame)
		{
			// Now enabled
			this->mEnabled = true ;
			
			// Call event
			this->OnEnabled(pTime, pCAN, pReadFrame) ;
		}
		
		/**
		 *	Disable the module
		 *	@param[in] pTime		Current time in ms
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		void Disable(unsigned long pTime, CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame)
		{
			// Now disabled
			this->mEnabled = false ;
			
			// Call event
			this->OnDisabled(pTime, pCAN, pReadFrame) ;
		}

		/**
		 *	Update method (to call each frame)
		 *	@param[in] pTime		Current time in ms
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		void Update(unsigned long pTime, CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame)
		{
			// Enabled ?
			if (this->IsEnabled())
			{
				// Update rate reached ?
				if ((unsigned long)(pTime - this->mLastUpdateTime) >= this->GetUpdateRate())
				{
					// Now updated
					this->mLastUpdateTime = pTime ;
					
					// Call implementation
					this->UpdateImpl(pTime, pCAN, pReadFrame) ;
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
		 *	@param[in] pTime		Current time in ms
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		virtual void OnEnabled(unsigned long pTime, CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame) 
		{
		}
		
		/**
		 *	Event called when the module is disabled
		 *	@param[in] pTime		Current time in ms
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		virtual void OnDisabled(unsigned long pTime, CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame) 
		{
		}
	
		/**
		 *	Update method implementation
		 *	@param[in] pTime		Current time in ms
		 *	@param[in] pCAN			CAN connector to use
		 *	@param[in] pReadFrame	Read frame to use
		 */
		virtual void UpdateImpl(unsigned long pTime, CAN::ICANConnector& pCAN, CAN::CReadCANFrame& pReadFrame) = 0 ;

	private:
		bool 			mEnabled ;			/**< If the module is enabled or not */
		unsigned long	mUpdateRate ;		/**< Current update rate */
		unsigned long	mLastUpdateTime ;	/**< Last update time */
	} ;
}