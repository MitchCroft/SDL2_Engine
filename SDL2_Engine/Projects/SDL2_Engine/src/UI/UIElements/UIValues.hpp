#pragma once

//! Define a maximum length for Virtual Axis names
#ifdef UI_TAG_LENGTH_MAX
#undef UI_TAG_LENGTH_MAX
#endif
#define UI_TAG_LENGTH_MAX 32

//! Include the SDL2_Engine objects
#include "../../Utilities/FString.hpp"

namespace SDL2_Engine {
	namespace UI {
		//! Export the required FString implementation
		template class SDL2_LIB_INC Utilities::FString<UI_TAG_LENGTH_MAX>;

		//! Create a base typedef for Tag strings
		typedef Utilities::FString<UI_TAG_LENGTH_MAX> uiTag;

		//! Define the basic UI elements that can be created
		enum class EUIType { Panel, Label, Button, Textbox };
	}
}