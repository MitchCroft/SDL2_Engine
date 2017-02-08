#pragma once

#include <thread>
#include <atomic>

#include "../../Types.hpp"

#include "../StateManagerValues.hpp"
#include "../StateManagerForwardDeclares.hpp"

namespace SDL2_Engine {
	namespace StateManager {
		/*
		 *		Name: StateManager
		 *		Author: Mitchell Croft
		 *		Created: 08/02/2017
		 *		Modified: 08/02/2017
		 *
		 *		Purpose:
		 *		Manage a pair of functions and Time values in order
		 *		to form a multi-threaded game loop that can be stopped
		 *		from any position
		**/
		class StateManager {
			//! Store singleton values
			static StateManager* mInstance;
			StateManager();
			~StateManager() = default;

			//! Store a flag to indicate that the game loop should still run
			std::atomic<bool> mRunning;

			//! Store a reference to the thread that is running
			std::thread mThread;

			//! Store the time values
			Time* mMainTime;
			Time* mThreadTime;

			//! Store pointers to the functions to execute
			StateManagerInitFunc mInitFunc;
			StateManagerUpdateFunc mMainFunc;
			StateManagerUpdateFunc mThreadFunc;
			StateManagerDestroyFunc mDestroyFunc;

			//! Store a pointer to the number of command line parameters
			char** mCmdArgs;
			int mCmdCount;

			//! Store debug output information
			DebugCallback mWarningCB;
			DebugCallback mErrorCB;

		public:
			//! Main functions
			static bool init(StateManagerInitValues pSettings);
			static void run();
			static void destroy();

			//! Use this to quit operation
			static inline void quit() { mInstance->mRunning.store(false); }

			//! Access the stored command line arguments
			static inline const char* const* getCmdArgs() { return mInstance->mCmdArgs; }
			static inline const int& getCmdCount() { return mInstance->mCmdCount; }
		};
	}
}