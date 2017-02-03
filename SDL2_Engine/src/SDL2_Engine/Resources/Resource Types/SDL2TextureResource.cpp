#include "SDL2TextureResource.hpp"

//! Get access to the SDL functions
#include <SDL.h>

namespace SDL2_Engine {
	namespace Resources {
		/*
			__SDL2Resource (Texture) : freeMemory - Clear the managed Texture resource
			Author: Mitchell Croft
			Created: 02/02/2017
			Modified: 03/02/2017
		*/
		void __SDL2Resource<Texture>::freeMemory() {
			//Check texture exists before removing it
			if (mTexture) {
				//Destroy the texture
				SDL_DestroyTexture(mTexture);

				//Reset the texture pointer
				mTexture = nullptr;

				//Update the resource status
				mStatus = EResourceLoadStatus::Freed;
			}
		}
	}
}