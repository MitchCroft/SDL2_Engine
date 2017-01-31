#include "TimeManager.h"

/*
	validateScale - Simple function used for ensuring the time scale value is always >= 0
	Author: Mitchell Croft
	Created: 20/01/2017
	Modified: 20/01/2016

	param[in] const float& pValue - The value that must be validated and returned to the ValidationProperty object

	return float - Returns pValue, clamped to a positive number
*/
inline float validateScale(const float& pValue) { return pValue < 0.f ? 0.f : pValue; }

namespace SDL2_Engine {
	//! Define the static Time instance
	Time Time::mInstance;

	/*
		Time : Constructor - Initialise the Time object with references back to the singleton
		Author: Mitchell croft
		Created: 20/01/2017
		Modified: 20/01/2017
	*/
	Time::Time() :
		mData(mInstance.mData ? nullptr : new TimeInformation()),
		deltaTime(mInstance.mData->deltaTime),
		realDeltaTime(mInstance.mData->realDeltaTime),
		timeScale(mInstance.mData->timeScale, validateScale),
		elapsedTime(mInstance.mData->elapsedTime),
		realElapsedTime(mInstance.mData->realElapsedTime) {

		//If this object is the singleton instance, get the starting time values
		if (mData) {
			//Get the performance frequency of the CPU
			QueryPerformanceFrequency(&mData->performanceFrequency);

			//Get the starting time
			QueryPerformanceCounter(&mData->startTime);

			//Set the starting time values
			mData->deltaTime = mData->realDeltaTime = 0.f;
			mData->elapsedTime = mData->realElapsedTime = 0.0;

			//Set the starting time scale value
			mData->timeScale = 1.f;
		}
	}

	/*
		Time : Destructor - Delete allocated memory
		Author: Mitchell Croft
		Created: 20/01/2017
		Modified: 20/01/2017
	*/
	Time::~Time() { delete mData; }

	/*
		Time : update - Update the singleton instance's time values
		Author: Mitchell Croft
		Created: 20/01/2017
		Modified: 20/01/2017
	*/
	void Time::update() {
		//Get the finishing time
		QueryPerformanceCounter(&mInstance.mData->endTime);

		//Calculate the real delta time
		mInstance.mData->realDeltaTime = (mInstance.mData->endTime.QuadPart - mInstance.mData->startTime.QuadPart) /
			(float)mInstance.mData->performanceFrequency.QuadPart;

		//Scale the real delta time
		mInstance.mData->deltaTime = mInstance.mData->realDeltaTime * mInstance.mData->timeScale;

		//Add to the delta time values to the elapsed time values
		mInstance.mData->elapsedTime += mInstance.mData->deltaTime;
		mInstance.mData->realElapsedTime += mInstance.mData->realDeltaTime;

		//Store the new start time
		QueryPerformanceCounter(&mInstance.mData->startTime);
	}
}