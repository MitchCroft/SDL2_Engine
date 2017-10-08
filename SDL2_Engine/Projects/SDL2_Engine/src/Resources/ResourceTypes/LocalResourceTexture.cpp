#include "LocalResourceTexture.hpp"

//! Include logging capabilities
#include "../../Globals.hpp"
#include "../../Debug/Logger.hpp"

//! Include the SDL2 definitions
#include <SDL.h>
#include <SDL_image.h>

namespace SDL2_Engine {
	namespace ResourceTypes {
		/*
			LocalResource (Texture) : dispose - Unload SDL2 resource information
			Created: 04/10/2017
			Modified: 04/10/2017
		*/
		void __LocalResource<Texture>::dispose() {
			//Check there is a texture to remove
			if (texture) {
				//Destroy the texture
				SDL_DestroyTexture(texture);

				//Reset the pointer
				texture = nullptr;

				//Set the status flag
				mStatus = EResourceLoadStatus::Unloaded;
			}
		}

		/*
			LocalResource (Texture) : Constructor - Initialise with default values
			Created: 04/10/2017
			Modified: 04/10/2017

			param[in] pRenderer - The SDL2 renderer object that will be used to create the texture
			param[in] pPath - The path of the image file to load
			param[in] pBlendMode - The SDL_BlendMode value to be applied to the texture
		*/
		__LocalResource<Texture>::__LocalResource(SDL_Renderer* pRenderer, const char* pPath, const int& pBlendMode /* = 0*/) : LocalResourceBase(EResourceType::Texture), texture(nullptr) {
			//Load the texture from the file
			texture = IMG_LoadTexture(pRenderer, pPath);

			//Check the texture was loaded properly
			if (!texture) {
				//Output error message
				Globals::get<Debug::Logger>().logError("Local Resource (Texture) failed to load the texture '%s'. Error: %s", pPath, pBlendMode, SDL_GetError());

				//Flag error status
				mStatus = EResourceLoadStatus::Error;
			}

			//Set the blend mode
			else if (pBlendMode && SDL_SetTextureBlendMode(texture, (SDL_BlendMode)pBlendMode)) {
				//Output error message
				Globals::get<Debug::Logger>().logError("Local Resource (Texture) failed to load the file '%s' with the blend mode '%i'. Error: %s", pPath, pBlendMode, SDL_GetError());

				//Set the error state
				mStatus = EResourceLoadStatus::Error;

				//Free the texture
				SDL_DestroyTexture(texture);

				//Reset the texture pointer
				texture = nullptr;
			}

			//Otherwise assign loaded flag
			else mStatus = EResourceLoadStatus::Loaded;
		}
	}
}