#pragma once

#include <cstdio>

//! Include required values
#include "../Utilities/Bitmask.hpp"
#include "WindowValues.hpp"

namespace SDL2_Engine {
	namespace Initialisation {
		/*
		 *		Name: WindowInitialiser
		 *		Author: Mitchell Croft
		 *		Created: 06/10/2017
		 *		Modified: 06/10/2017
		 *
		 *		Purpose:
		 *		Store initialisation values for the Window object
		**/
		struct WindowInitialiser {
			//! The name to be applied to the window of the application
			char title[FILENAME_MAX];

			//! The dimensions to initialise the window with
			size_t width, height;

			//! Indicate the style the window should be created in
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