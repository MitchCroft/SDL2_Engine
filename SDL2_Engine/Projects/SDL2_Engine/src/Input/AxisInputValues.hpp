#pragma once

#define VIRTUAL_AXIS_NAME_MAX 32

//! Include SDL2_Engine objects
#include "../Utilities/FString.hpp"

namespace SDL2_Engine {
	namespace Input {
		//! Export the required FString implementation
		template class SDL2_LIB_INC Utilities::FString<VIRTUAL_AXIS_NAME_MAX>;

		//! Create a type define for the names of Virtual Axis
		typedef Utilities::FString<VIRTUAL_AXIS_NAME_MAX> vAxisName;

		//! Flag the input devices the Virtual Axis can use
		enum class EInputDevice { Null, GamePad, Keyboard, Mouse };
	}
}