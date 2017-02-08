#include "StateManager.hpp"

//! Included to use variable argument lists
#include <cstdarg>

//! Include the Time object
#include "../../Root/Time.hpp"

//! Include the State Manager initialisation value object
#include "StateManagerInitValues.hpp"

namespace SDL2_Engine {
	namespace StateManager {
		//! Define statics
		StateManager* StateManager::mInstance = nullptr;

		/*
			simpleOutput - Provide a very basic, simple output to be used in the event that
						   no DebugCallbacks are supplied during initialisation
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017

			param[in] pFormat - A c-string with the format symbols defined in
			http://www.cplusplus.com/reference/cstdio/fprintf/
			param[in] Additional Values - The values to be formatted into the c-string
		*/
		inline void simpleOutput(const char* pFormat, ...) {
			va_list args;
			va_start(args, pFormat);
			vprintf((std::string(pFormat) + '\n').c_str(), args);
			va_end(args);
		}
	
		/*
			StateManager : Constructor - Initialise with default values
			Author: Mitchell Croft
			Created: 08/02/2017
			Modified: 08/02/2017
		*/
		StateManager::StateManager() :
			mRunning(false),
			mMainTime(nullptr),
			mThreadTime(nullptr),
			mInitFunc(nullptr),
			mMainFunc(nullptr),
			mThreadFunc(nullptr),
			mDestroyFunc(nullptr),
			mCmdArgs(nullptr),
			mCmdCount(0),
			mWarningCB(nullptr),
			mErrorCB(nullptr)
		{}

		/*
			StateManager : init - Construct the singleton with setup values
			Author: Mitchell Croft
			Created: 08/02/2017
			Modified: 08/02/2017

			param[in] pSettings - A StateManagerInitValues object defining the values to initialise
								  the State Manager with

			return bool - Returns true if the StateManager initialised correctly
		*/
		bool StateManager::init(StateManagerInitValues pSettings) {
			//Check the debug outputs are valid
			if (!pSettings.warningCallback) pSettings.warningCallback = simpleOutput;
			if (!pSettings.errorCallback) pSettings.errorCallback = pSettings.warningCallback;

			//Check if the instance has already been created
			if (mInstance) {
				//Output warning
				mInstance->mWarningCB("Attempted to re-create the SDL2_Engine::StatManager singleton after it had been initialised");

				//Return failure
				return false;
			}

			//Create the State Manager singleton instance
			if (!(mInstance = new StateManager())) {
				//Output error
				pSettings.errorCallback("Failed to create the SDL2_Engine::StateManager singleton");

				//Return failure
				return false;
			}

			//Assign the command line argument values
			mInstance->mCmdArgs = pSettings.commandArgs;
			mInstance->mCmdCount = pSettings.commandArgsCount;

			//Assign the function pointers
			mInstance->mInitFunc = pSettings.initFunc;
			mInstance->mMainFunc = pSettings.mainFunc;
			mInstance->mThreadFunc = pSettings.threadFunc;
			mInstance->mDestroyFunc = pSettings.destroyFunc;

			//Assign the debug callback functions
			mInstance->mWarningCB = pSettings.warningCallback;
			mInstance->mErrorCB = pSettings.errorCallback;

			//Return success
			return true;
		}

		/*
			StateManager : run - Start executing the multi-threaded game loop using the supplied function pointers
			Author: Mitchell Croft
			Created: 08/02/2017
			Modified: 08/02/2017

			Note:
			This function will return immediately if there is a thread function setup without a main. This function
			will hold the entire thread up if a main function is supplied. These loops can be broken by calling 
			StateManager::quit()
		*/
		void StateManager::run() {
			//Check if the StateManager is already running
			if (mInstance->mRunning) {
				//Output warning
				mInstance->mWarningCB("Attempted to run the SDL2_Engine::StateManager while it is already running");

				//Leave the function
				return;
			}

			//Check there are functions to run
			else if (!mInstance->mMainFunc && !mInstance->mThreadFunc) {
				//Output error
				mInstance->mErrorCB("Attempted to run the SDL2_Engine::StateManager with no game loop functions assigned. Failed to start");

				//Leave the function
				return;
			}

			//Check the initialisation function
			else if (mInstance->mInitFunc && !mInstance->mInitFunc()) {
				//Output error
				mInstance->mErrorCB("SDL2_Engine::StateManager failed the initialisation function");

				//Leave the function
				return;
			}

			//Setup the separate thread
			if (mInstance->mThreadFunc) {
				//Create the thread object
				mInstance->mThread = std::thread([&]() {
					//Hold the thread until the main function is ready to loop
					while (!mInstance->mRunning);

					//Create the thread time object
					mInstance->mThreadTime = new Time();

					//Start the loop
					do {
						//Update the time values
						mInstance->mThreadTime->update();

						//Call the function
						mInstance->mThreadFunc(*mInstance->mThreadTime);
					} while (mInstance->mRunning.load());
				});
			}

			//Output warning message
			else mInstance->mWarningCB("Running the SDL2_Engine::StateManager without setting up a seperate thread loop");

			//Setup the main thread
			if (mInstance->mMainFunc) {
				//Flag the state manager as running
				mInstance->mRunning = true;

				//Create the main time object
				mInstance->mMainTime = new Time();

				//Start the loop 
				do {
					//Update the time values
					mInstance->mMainTime->update();

					//Call the function
					mInstance->mMainFunc(*mInstance->mMainTime);
				} while (mInstance->mRunning.load());
			}

			else {
				//Output warning message
				mInstance->mWarningCB("Running the SDL2_Engine::StateManager without setting up a main thread loop");

				//Flag the state manager as running
				mInstance->mRunning = true;
			}
		}

		/*
			StateManager : destroy - Destroy allocated memory and terminate the game loops
			Author: Mitchell Croft
			Created: 08/02/2017
			Modified: 08/02/2017
		*/
		void StateManager::destroy() {
			//Check the instance has been created
			if (mInstance) {
				//Check if the StateManager is still running
				if (mInstance->mRunning) 
					mInstance->mWarningCB("Destroying the SDL2_Engine::StateManager while it was still running");

				//Reset the running flag
				mInstance->mRunning = false;

				//Attempt to join the separate thread
				if (mInstance->mThread.get_id() != std::thread::id())
					mInstance->mThread.join();

				//Delete the Time objects
				if (mInstance->mMainTime) delete mInstance->mMainTime;
				if (mInstance->mThreadTime) delete mInstance->mThreadTime;

				//Check the destroy function
				if (mInstance->mDestroyFunc) mInstance->mDestroyFunc();

				//Reset the singleton instance
				delete mInstance;
				mInstance = nullptr;
			}
		}
	}
}