#pragma once

//! Include the library management definitions
#include "__LibraryManagement.hpp"

//! Include the Singleton interface
#include "Utilities/IGlobal.hpp"

//! Include time-keeping values 
#include <windows.h>

namespace SDL2_Engine {
	/*
	 *		Name: Time
	 *		Author: Mitchell Croft
	 *		Created: 19/07/2017
	 *		Modified: 06/10/2017
	 *		
	 *		Purpose:
	 *		Track changes in time over the course of the programs
	 *		execution
	**/
	class SDL2_LIB_INC Time : public Utilities::IGlobal {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-------------------------------------Get Time Values---------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/*
			Time : getDelta - Get the scaled delta time of the last cycle
			Created: 19/07/2017
			Modified: 19/07/2017

			return const float& - Returns the delta time as constant float reference (in seconds)
		*/
		inline const float& getDelta() const noexcept { return mDeltaTime; }

		/*
			Time : getRealDelta - Get the unscaled delta time of the last cycle
			Created: 19/07/2017
			Modified: 19/07/2017

			return const float& - Returns the delta time as constant float reference (in seconds)
		*/
		inline const float& getRealDelta() const noexcept { return mRealDeltaTime; }

		/*
			Time : getElapsed - Get the scaled elapsed time since the Time objects creation
			Created: 19/07/2017
			Modified: 19/07/2017

			return const double& - Returns the elapsed time as constant double reference (in seconds)
		*/
		inline const double& getElapsed() const noexcept { return mElapsedTime; }

		/*
			Time : getRealElapsed - Get the unscaled elapsed time since the Time objects creation
			Created: 19/07/2017
			Modified: 19/07/2017

			return const double& - Returns the elapsed time as constant double reference (in seconds)
		*/
		inline const double& getRealElapsed() const noexcept { return mRealElapsedTime; }

		/*
			Time : getScale - Get the current scale value that is being applied to the time values
			Created: 19/07/2017
			Modified: 19/07/2017

			return const float& - Returns the current scale value as a constant float reference that is >= 0
		*/
		inline const float& getScale() const noexcept { return mTimeScale; }

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-------------------------------------Set Time Values---------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/*
			Time : setScale - Set the scale value that is applied to the time values
			Created: 19/07/2017
			Modified: 19/07/2017

			param[in] pScl - A float value that is >= 0 reflecting the scale that should be applied to passed time
		*/
		inline void setScale(const float& pScl) noexcept { mTimeScale = (pScl > 0.f ? pScl : 0.f); }

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------Construction/Destruction-----------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Time : Constructor - Initialise with default values
			Created: 19/07/2017
			Modified: 22/09/2017
		*/
		Time();

		/*
			Time : createInterface - Blank function, no setup required for the Time object
			Created: 19/07/2017
			Modified: 19/07/2017

			return bool - Always returns true
		*/
		inline bool createInterface() override { return true; }

		/*
			Time : destroyInterface - Blank function, no destruction required for the Time object
			Created: 19/07/2017
			Modified: 19/07/2017
		*/
		inline void destroyInterface() override {}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------------Main Function----------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Time : update - Update the internal time values
			Created: 19/07/2017
			Modified: 06/10/2017
		*/
		void update() override;

	public:
		//! Store the performance frequency of the CPU
		LARGE_INTEGER mPerformanceFreq;

		//! Store the start and end times for tracking delta time
		LARGE_INTEGER mStartTime, mEndTime;

		//! Store the various delta time values
		float mDeltaTime, mRealDeltaTime;

		//! Store the elapsed time values
		double mElapsedTime, mRealElapsedTime;

		//! Store the scale applied to the passage of time
		float mTimeScale;
	};
}