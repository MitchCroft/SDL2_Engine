#pragma once

//! Include required values
#include "../__LibraryManagement.hpp"
#include "../Utilities/Bitmask.hpp"
#include "RenderingValues.hpp"

namespace SDL2_Engine {
	//! Prototype the ISceneRenderer object
	namespace Rendering { class ISceneRenderer; }

	namespace Initialisation {
		/*
		 *		Name: RendererInitialiser
		 *		Author: Mitchell Croft
		 *		Created: 09/10/2017
		 *		Modified: 09/10/2017
		 *		
		 *		Purpose:
		 *		Store initialisation values for the Rendering object
		**/
		struct SDL2_LIB_INC RendererInitialiser {
			//! The index of the graphics device to initialise (Defaults to -1, the first to support the requested flags)
			int deviceIndex;

			//! A bitmask of ERendererProperties to be used in the creation of the Renderer
			Utilities::Bitmask<Rendering::ERendererProperties> properties;

			//! Store a pointer to the initial ISceneRenderer object
			Rendering::ISceneRenderer* sceneRenderer;

			/*
				RendererInitialiser : Constructor - Initialise with default values
				Created: 09/10/2017
				Modified: 09/10/2017
			*/
			RendererInitialiser();
		};
	}
}