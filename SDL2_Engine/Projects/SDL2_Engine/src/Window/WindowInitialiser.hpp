#pragma once

//! Include SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "../Utilities/Bitmask.hpp"
#include "../Utilities/FString.hpp"
#include "WindowValues.hpp"

//! Get the maximum length of a file name
#include <cstdio>

namespace SDL2_Engine {
	namespace Initialisation {
		//! Export the required FString implementation
		template class SDL2_LIB_INC Utilities::FString<FILENAME_MAX>;

		/*
		 *		Name: WindowInitialiser
		 *		Author: Mitchell Croft
		 *		Created: 06/10/2017
		 *		Modified: 10/10/2017
		 *
		 *		Purpose:
		 *		Store initialisation values for the Window object
		**/
		struct SDL2_LIB_INC WindowInitialiser {
			//! The name to be applied to the window of the application (Default "SDL2_Engine Application")
			Utilities::FString<FILENAME_MAX> title;

			//! The dimensions to initialise the window with (Default 1280x720)
			size_t width, height;

			//! Indicate the style the window should be created in (Default Style_Resizeable)
			Utilities::Bitmask<EWindowProperties> properties;

			/*
				WindowInitialiser : Constructor - Initialise with default values
				Created: 06/10/2017
				Modified: 06/10/2017
			*/
			WindowInitialiser();
		};
	}
}