#include "LocalResourceFont.hpp"

//! Include logging capabilities
#include "../../Globals.hpp"
#include "../../Debug/Logger.hpp"

//! Include the SDL2 definitions
#include <SDL.h>
#include <SDL_ttf.h>

namespace SDL2_Engine {
	namespace ResourceTypes {
		/*
			LocalResource (Font) : dispose - Unload SDL2 resource information
			Created: 05/10/2017
			Modified: 05/10/2017
		*/
		void __LocalResource<Font>::dispose() {
			//Check the font exists
			if (font) {
				//Close the font
				TTF_CloseFont(font);

				//Reset the pointer
				font = nullptr;

				//Flag as unloaded
				mStatus = EResourceLoadStatus::Freed;
			}
		}

		/*
			LocalResource (Font) : Constructor - Initialise with default values
			Created: 05/10/2017
			Modified: 08/11/2017

			param[in] pPath - The path of the font file to load
			param[in] pSize - The point size to use for the font
			param[in] pIndex - Optional index of the font face to load from the file (Default 0)
		*/
		__LocalResource<Font>::__LocalResource(const char* pPath, const int& pSize, const long& pIndex /* = 0*/) : ILocalResourceBase(EResourceType::Font), font(nullptr), fontSize(pSize) {
			//Load the font
			font = TTF_OpenFontIndex(pPath, pSize, pIndex);

			//Check the file was loaded properly
			if (!font) {
				//Output error message
				Globals::get<Debug::Logger>().logError("Local Resource (Font) failed to load the font file '%s' with size %i and index %i. Error: %s", pPath, pSize, pIndex, SDL_GetError());

				//Flag error status
				mStatus = EResourceLoadStatus::Error;
			}

			//Otherwise assign loaded flag
			else mStatus = EResourceLoadStatus::Loaded;
		}
	}
}