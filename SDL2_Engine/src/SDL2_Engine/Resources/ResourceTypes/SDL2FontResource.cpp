#include "SDL2FontResource.hpp"

//! Include the Font String Resource
#include "SDL2FontStringResource.hpp"

//! Include the SDL true type font
#include <SDL_ttf.h>

namespace SDL2_Engine {
	namespace Resources {
		namespace ResourceTypes {
			/*
				__SDL2Resource (Font) : freeMemory - Clear the managed Font resource
				Author: Mitchell Croft
				Created: 08/03/2017
				Modified: 08/03/2017
			*/
			void __SDL2Resource<Font>::freeMemory() {
				//Check if the font exists
				if (mFont) {
					//Free the font memory
					TTF_CloseFont(mFont);

					//Nullify the resource
					mFont = nullptr;

					//Update the resource status
					mStatus = EResourceLoadStatus::Freed;
				}
			}

			/*
				__SDL2Resource (Font) : renderString - Render the passed in character string using the current Font
				Author: Mitchell Croft
				Created: 08/03/2017
				Modified: 15/03/2017

				param[in] pString - The character string to render
				param[in] pTransparency - Flags if the text should be rendered with transparency

				return SDL2Resource<FontString> - Returns an SDL2Resource<FontString> object set up with the rendered text
			*/
			SDL2Resource<FontString> __SDL2Resource<Font>::renderString(const char* pString, const bool& pTransparency /* = false */) {
				//Create a new texture resource
				SDL2Resource<FontString> newTex = SDL2Resource<FontString>(new __SDL2Resource<FontString>());

				//Assign the current status
				newTex->mStatus = mStatus;

				//Check the Font has been loaded
				if (mStatus == EResourceLoadStatus::Loaded) {
					//Render the text to a surface
					SDL_Surface* tempSurface = (pTransparency ? TTF_RenderText_Blended : TTF_RenderText_Solid)(mFont, pString, { 255,255,255,255 });

					//Check the surface was rendered
					if (!tempSurface) {
						newTex->mStatus = EResourceLoadStatus::Error;
						mErrorCB("Failed to render text '%s' to a surface using the font with ID %i. ERROR: %s", pString, mID, TTF_GetError());
						return newTex;
					}

					//Assign the surface to the FontString
					newTex->mText = tempSurface;
				}

				//Return the generated texture
				return newTex;
			}
		}
	}
}