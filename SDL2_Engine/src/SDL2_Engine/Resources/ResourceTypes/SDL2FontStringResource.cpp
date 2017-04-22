#include "SDL2FontStringResource.hpp"

//! Include SDL for freeing functions
#include <SDL.h>

namespace SDL2_Engine {
	namespace Resources {
		namespace ResourceTypes {
			/*
				__SDL2Resource (FontString) : freeMemory - Clear the managed Font String resource
				Author: Mitchell Croft
				Created: 08/03/2017
				Modified: 08/03/2017
			*/
			void __SDL2Resource<FontString>::freeMemory() {
				//Check if the surface exists
				if (mText) {
					//Free the surface
					SDL_FreeSurface(mText);

					//Reset the pointer
					mText = nullptr;

					//Set the status
					mStatus = EResourceLoadStatus::Freed;
				}
			}
		}
	}
}
