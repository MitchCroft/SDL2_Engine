#include "SDL2MusicResource.hpp"

//! Include SDL_Mixer to get access to the destroy music function
#include <SDL_mixer.h>

namespace SDL2_Engine {
	namespace Resources {
		namespace ResourceTypes {
			/*
				SDL2Resource (Music) : freeMemory - Clear the managed Mix Music resource
				Author: Mitchell Croft
				Created: 02/02/2017
				Modified: 03/02/2017
			*/
			void __SDL2Resource<Music>::freeMemory() {
				//Check the music exists before removing it
				if (mMusic) {
					//Destroy the music
					Mix_FreeMusic(mMusic);

					//Reset the music pointer
					mMusic = nullptr;

					//Update the resources status
					mStatus = EResourceLoadStatus::Freed;
				}
			}
		}
	}
}