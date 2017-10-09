#include "RendererInitialiser.hpp"

namespace SDL2_Engine {
	namespace Initialisation {
		/*
			RendererInitialiser : Constructor - Initialise with default values
			Created: 09/10/2017
			Modified: 09/10/2017
		*/
		RendererInitialiser::RendererInitialiser() :
			deviceIndex(-1),
			properties(Rendering::ERendererProperties::Hardware_Acceleration, Rendering::ERendererProperties::VSync),
			sceneRenderer(nullptr)
		{}
	}
}
