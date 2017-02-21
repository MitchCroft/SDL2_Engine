#pragma once

//! Include the type information
#include "../Types.hpp"

//! Include the State Manager function pointer defines
#include "StateManagerValues.hpp"

namespace SDL2_Engine {
	/*
	 *		Name: StateManagerInitValues
	 *		Author: Mitchell Croft
	 *		Created: 08/02/2017
	 *		Modified: 08/02/2017
	 *
	 *		Purpose:
	 *		Provide setup instructions to the SDL2_Engine::StateManager
	 *		initialisation function
	**/
	struct StateManagerInitValues {
		///////////////////////////////////////////////////////////////////////////////////////////////
		//// Values below are used to store command line argument information in the State Manager ////
		///////////////////////////////////////////////////////////////////////////////////////////////

		//! Store a pointer to the command line arguments
		char** commandArgs = nullptr;

		//! Store the number of command line arguments within the pointer
		int commandArgsCount = 0;

		///////////////////////////////////////////////////////////////////////////////////////////////
		////      Values below are used execute user code at different points of the game loop     ////
		///////////////////////////////////////////////////////////////////////////////////////////////

		//! Store pointers to the functions to execute
		StateManagerInitFunc initFunc = nullptr;
		StateManagerUpdateFunc mainFunc = nullptr;
		StateManagerUpdateFunc threadFunc = nullptr;
		StateManagerDestroyFunc destroyFunc = nullptr;

		///////////////////////////////////////////////////////////////////////////////////////////////
		////        Values below are used to indicate were debug information should be sent        ////
		///////////////////////////////////////////////////////////////////////////////////////////////

		//! Store the debug output options
		DebugCallback warningCallback = nullptr;
		DebugCallback errorCallback = nullptr;
	};
}
