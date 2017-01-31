#pragma once

//! Include time-keeping values 
#include <windows.h>

//! Include the properties to allow for managed property values
#include "../../Utilities/Properties.h"

namespace SDL2_Engine {

	/*
	 *		Name: Time
	 *		Author: Mitchell Croft
	 *		Created: 20/01/2017
	 *		Modified: 20/01/2017
	 *		
	 *		Purpose:
	 *		Store and track values related to time. Time values
	 *		can be read (but not modified) through the public properties.
	 *		All time values are in seconds. The update function should be 
	 *		called once (and only once) per update cycle to keep the time 
	 *		values accurate. 
	**/
	class Time {
		//! Store the singleton instance
		static Time mInstance;

		//! Prototype a struct to hold Time information
		struct TimeInformation;

		//! Store a pointer to a TimeInformation struct
		TimeInformation* mData;

	public:
		//! Constructor / Destructor
		Time();
		~Time();

		//! Update function, updates the time since the last cycle
		void update();

		//! Expose the time values that can be used by the user
		Utilities::Properties::ReadOnlyProperty<float> deltaTime;
		Utilities::Properties::ReadOnlyProperty<float> realDeltaTime;

		Utilities::Properties::ValidatedProperty<float> timeScale;

		Utilities::Properties::ReadOnlyProperty<double> elapsedTime;
		Utilities::Properties::ReadOnlyProperty<double> realElapsedTime;
	};

	/*
	 *		Name: TimeInformation
	 *		Author: Mitchell Croft
	 *		Created: 20/01/2017
	 *		Modified: 20/01/2017
	 *		
	 *		Purpose:
	 *		Store time values separate from the main Time class
	 *		to reduce the size of the object whenever a new instance
	 *		is created in order to gain access to the time information
	**/
	struct Time::TimeInformation {
		//! Store the performance frequency of the CPU
		LARGE_INTEGER performanceFrequency;

		//! Store the start and ending times for tracking delta time
		LARGE_INTEGER startTime, endTime;

		//! Store the various delta time values
		float deltaTime, realDeltaTime;

		//! Store the time scale to apply to delta time
		float timeScale;

		//! Store the time that has passed accumulatively
		double elapsedTime, realElapsedTime;
	};
}