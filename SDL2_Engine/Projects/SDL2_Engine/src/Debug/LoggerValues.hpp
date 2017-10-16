#pragma once

//! Include the SDL2_objects
#include "../Utilities/FString.hpp"

//! Get the maximum length of a file name
#include <cstdio>

namespace SDL2_Engine {
	namespace Debug {
		//! Export the required FString implementation
		template class SDL2_LIB_INC Utilities::FString<FILENAME_MAX>;

		//Create a basic type define for a filepath
		typedef Utilities::FString<FILENAME_MAX> filepath;
	}
}