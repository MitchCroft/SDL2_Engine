#pragma once

//! Include the SDL2_Engine objects
#include "../__LibraryManagement.hpp"

namespace SDL2_Engine {
	//! Prototype the VirtualAxis
	namespace Input { struct VirtualAxis; }

	namespace Initialisation {
		/*
		 *		Name: AxisInputInitialiser
		 *		Author: Mitchell Croft
		 *		Created: 12/10/2017
		 *		Modified: 12/10/2017
		 *		
		 *		Purpose:
		 *		Store initialisation settings for the Input object
		**/
		struct SDL2_LIB_INC AxisInputInitialiser {
			//! Store a pointer to an array of default Virtual Input Axis (Default nullptr)
			Input::VirtualAxis* defaultAxis;
			
			//! Store the number of Virtual Input Axis that are in the array
			size_t count;

			/*
				AxisInputInitialiser : Constructor - Initialise with default values
				Created: 12/10/2017
				Modified: 12/10/2017
			*/
			AxisInputInitialiser();
		};
	}
}