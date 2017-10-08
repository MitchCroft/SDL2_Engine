#include "WindowInitialiser.hpp"

namespace SDL2_Engine {
	namespace Initialisation {
		/*
			WindowInitialiser : Constructor - Initialise with default values
			Created: 06/10/2017
			Modified: 06/10/2017
		*/
		WindowInitialiser::WindowInitialiser() :
			title("SDL2_Engine Application"),
			width(1280), height(720),
			properties(EWindowProperties::Style_Resizeable)
		{}
	}
}
