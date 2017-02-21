#include "Time.hpp"

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
	namespace Time {
		/*
			Time : Constructor - Initialise the Time object with references back to the singleton
			Author: Mitchell croft
			Created: 20/01/2017
			Modified: 08/02/2017
		*/
		Time::Time() :
			deltaTime(mDeltaTime),
			realDeltaTime(mRealDeltaTime),
			timeScale(mTimeScale, validateScale),
			elapsedTime(mElapsedTime),
			realElapsedTime(mRealElapsedTime) {
			//Get the performance frequency of the CPU
			QueryPerformanceFrequency(&mPerformanceFrequency);

			//Get the starting time
			QueryPerformanceCounter(&mStartTime);

			//Set the starting time values
			mDeltaTime = mRealDeltaTime = 0.f;
			mElapsedTime = mRealElapsedTime = 0.0;

			//Set the starting time scale value
			mTimeScale = 1.f;
		}

		/*
			Time : update - Update the singleton instance's time values
			Author: Mitchell Croft
			Created: 20/01/2017
			Modified: 08/02/2017
		*/
		void Time::update() {
			//Get the finishing time
			QueryPerformanceCounter(&mEndTime);

			//Calculate the real delta time
			mRealDeltaTime = (mEndTime.QuadPart - mStartTime.QuadPart) /
				(float)mPerformanceFrequency.QuadPart;

			//Scale the real delta time
			mDeltaTime = mRealDeltaTime * mTimeScale;

			//Add to the delta time values to the elapsed time values
			mElapsedTime += mDeltaTime;
			mRealElapsedTime += mRealDeltaTime;

			//Store the new start time
			QueryPerformanceCounter(&mStartTime);
		}
	}
}