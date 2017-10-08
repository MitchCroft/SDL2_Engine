#include "LocalResourceSFX.hpp"

//! Include logging capabilities
#include "../../Globals.hpp"
#include "../../Debug/Logger.hpp"

//! Include the SDL2 definitions
#include <SDL.h>
#include <SDL_mixer.h>

namespace SDL2_Engine {
	namespace ResourceTypes {
		/*
			LocalResource (SFX) : dispose - Unload SDL2 resource information
			Created: 04/10/2017
			Modified: 04/10/2017
		*/
		void __LocalResource<SFX>::dispose() {
			//Check the chunk exists
			if (sfx) {
				//Free the audio chunk
				Mix_FreeChunk(sfx);

				//Reset the pointer
				sfx = nullptr;

				//Set the status flag
				mStatus = EResourceLoadStatus::Unloaded;
			}
		}

		/*
			LocalResource (SFX) : Constructor - Initialise with default values
			Created: 04/10/2017
			Modified: 04/10/2017

			param[in] pPath - The path of the sound effect file to load
		*/
		__LocalResource<SFX>::__LocalResource(const char* pPath) : LocalResourceBase(EResourceType::SFX), sfx(nullptr) {
			//Load the sound effect
			sfx = Mix_LoadWAV(pPath);

			//Check the file loaded properly
			if (!sfx) {
				//Output error message
				Globals::get<Debug::Logger>().logError("Local Resource (SFX) failed to load the sound effect file '%s'. Error: %s", pPath, SDL_GetError());

				//Flag error status
				mStatus = EResourceLoadStatus::Error;
			}

			//Otherwise assign loaded flag
			else mStatus = EResourceLoadStatus::Loaded;
		}
	}
}