#include "LocalResourceMusic.hpp"

//! Include logging capabilities
#include "../../Globals.hpp"
#include "../../Debug/Logger.hpp"

//! Include the SDL2 definitions
#include <SDL.h>
#include <SDL_mixer.h>

namespace SDL2_Engine {
	namespace ResourceTypes {
		/*
			LocalResource (Music) : dispose - Unload SDL2 resource information
			Created: 04/10/2017
			Modified: 04/10/2017
		*/
		void __LocalResource<Music>::dispose() {
			//Check the music exists
			if (music) {
				//Free the music object
				Mix_FreeMusic(music);

				//Reset the pointer
				music = nullptr;

				//Set the status flag
				mStatus = EResourceLoadStatus::Unloaded;
			}
		}

		/*
			LocalResource (Music) : Constructor - Initialise with default values
			Created: 04/10/2017
			Modified: 04/10/2017

			param[in] pPath - The path of the music file to load
		*/
		__LocalResource<Music>::__LocalResource(const char* pPath) : ILocalResourceBase(EResourceType::Music), music(nullptr) {
			//Load the sound effect
			music = Mix_LoadMUS(pPath);

			//Check the file loaded properly
			if (!music) {
				//Output error message
				Globals::get<Debug::Logger>().logError("Local Resource (Music) failed to load the music file '%s'. Error: %s", pPath, SDL_GetError());

				//Flag error status
				mStatus = EResourceLoadStatus::Error;
			}

			//Otherwise assign loaded flag
			else mStatus = EResourceLoadStatus::Loaded;
		}
	}
}