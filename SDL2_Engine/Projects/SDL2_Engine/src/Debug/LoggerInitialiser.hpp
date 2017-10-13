#pragma once

//! Include the SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "LoggerValues.hpp"

namespace SDL2_Engine {
	namespace Initialisation {
		/*
		 *		Name: LoggerInitialiser
		 *		Author: Mitchell Croft
		 *		Created: 06/10/2017
		 *		Modified: 13/10/2017
		 *		
		 *		Purpose:
		 *		Store initialisation values for the Logger object
		**/
		struct SDL2_LIB_INC LoggerInitialiser {
			//! Flag if the Logger should output to a log file (Default false)
			bool storeLog;

			//! Store the location to output the log file to (Default "./")
			Debug::filepath logLocation;

			/*
				LoggerInitialiser : Constructor - Initialise with default values
				Created: 06/10/2017
				Modified: 12/10/2017
			*/
			LoggerInitialiser();
		};
	}
}