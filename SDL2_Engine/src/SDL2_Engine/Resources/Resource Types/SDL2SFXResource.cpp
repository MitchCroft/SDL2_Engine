#include "SDL2SFXResource.hpp"

//! Include SDL_Mixer to get access to the destroy chunk function
#include <SDL_mixer.h>

namespace SDL2_Engine {
	namespace Resources {
		/*
			__SDL2Resource (SFX) : freeMemory - Clear the managed Mix Chunk resource
			Author: Mitchell Croft
			Created: 02/02/2017
			Modified: 03/02/2017
		*/
		void __SDL2Resource<SFX>::freeMemory() {
			//Check the SFX exists before removing it
			if (mSFX) {
				//Destroy the audio chunk
				Mix_FreeChunk(mSFX);

				//Reset the sound effect pointer
				mSFX = nullptr;

				//Update the resource status
				mStatus = EResourceLoadStatus::Freed;
			}
		}
	}
}
