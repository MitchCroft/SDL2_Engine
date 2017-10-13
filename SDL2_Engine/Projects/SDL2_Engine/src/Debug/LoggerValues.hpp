#pragma once

//! Include the SDL2_objects
#include "../Utilities/FString.hpp"

//! Get the maximum length of a file name
#include <cstdio>

namespace SDL2_Engine {
	namespace Debug {
		//Create a basic typedefine for a filepath
		typedef Utilities::FString<FILENAME_MAX> filepath;
	}
}