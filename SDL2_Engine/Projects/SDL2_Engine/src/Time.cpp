#include "Time.hpp"

namespace SDL2_Engine {
	/*
		Time : Constructor - Initialise with default values
		Created: 19/07/2017
		Modified: 22/09/2017
	*/
	Time::Time() :
		//Default the time values
		mDeltaTime(0.f), mRealDeltaTime(0.f),
		mElapsedTime(0.f), mRealElapsedTime(0.f),
		mTimeScale(1.f)	{
	
		//Get the performance frequency of the CPU
		QueryPerformanceFrequency(&mPerformanceFreq);

		//Get the Starting time
		QueryPerformanceCounter(&mStartTime);
	}

	/*
		Time : update - Update the internal time values
		Created: 19/07/2017
		Modified: 06/10/2017
	*/
	void Time::update() {
		//Get the finishing time
		QueryPerformanceCounter(&mEndTime);

		//Calculate the real delta time
		mRealDeltaTime = (mEndTime.QuadPart - mStartTime.QuadPart) / (float)mPerformanceFreq.QuadPart;

		//Cap the real delta time to 1
		mRealDeltaTime = (mRealDeltaTime < 1.f ? mRealDeltaTime : 1.f);

		//Scale the real delta time
		mDeltaTime = mRealDeltaTime * mTimeScale;

		//Add the delta time values to the elapsed time
		mElapsedTime += mDeltaTime;
		mRealElapsedTime += mRealDeltaTime;

		//Store the new start time
		QueryPerformanceCounter(&mStartTime);
	}
}