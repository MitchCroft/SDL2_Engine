#include "CanvasInitialiser.hpp"

namespace SDL2_Engine {
	namespace Initialisation {
		/*
			CanvasInitialiser : Constructor - Initialise with default values
			Created: 13/10/2017
			Modified: 13/10/2017
		*/
		CanvasInitialiser::CanvasInitialiser() :
			horizontalAxis("horizontal"),
			verticalAxis("vertical"),
			confirmAxis("confirm"),
			customLoader(nullptr),
			actionSetup(nullptr)
		{}
	}
}