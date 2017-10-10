#pragma once

#include <cstdio>

namespace SDL2_Engine {
	namespace Initialisation {
		/*
		 *		Name: LoggerInitialiser
		 *		Author: Mitchell Croft
		 *		Created: 06/10/2017
		 *		Modified: 10/10/2017
		 *		
		 *		Purpose:
		 *		Store initialisation values for the Logger object
		**/
		struct LoggerInitialiser {
			//! Flag if the Logger should output to a log file (Default false)
			bool storeLog;

			//! Store the location to output the log file to (Default "./")
			char logLocation[FILENAME_MAX];

			/*
				LoggerInitialiser : Constructor - Initialise with default values
				Created: 06/10/2017
				Modified: 06/10/2017
			*/
			LoggerInitialiser();
		};
	}
}