#include "BasicSceneRenderer.hpp"

//! Include the SDL2_Engine values
#include "../Globals.hpp"
#include "../Debug/Logger.hpp"
#include "../Utilities/Endian/Endian.hpp"

//! Include the SDL objects
#include <SDL.h>
#include <SDL_ttf.h>

namespace SDL2_Engine {
	namespace Rendering {
		/*
			BasicSceneRenderer : drawTexture - Render a texture using passed in values
			Created: 09/10/2017
			Modified: 09/10/2017

			param[in] pRenderer - The SDL Renderer object to be used to draw graphics to the screen
			param[in] pTexture - A pointer to the SDL_Texture object to render
			param[in] pPosition - An SDL_Rect object defining where the image is to be rendered
			param[in] pClip - A pointer to an SDL_Rect defining a section of the texture to render (Default nullptr)
			param[in] pFilter - A Colour object defining the filter to be applied to the rendered image (Default White)
			param[in] pAngle - A double value defining the rotation amount in degrees (Default 0)
			param[in] pPivot - A pointer to an SDL_Point object defining the pivot point for the rotation (Default nullptr)
			param[in] pFlip - The SDL_RendererFlip value defining if the image should be flipped in any way (Default SDL_FLIP_NONE)
		*/
		void BasicSceneRenderer::drawTexture(SDL_Renderer*& pRenderer, SDL_Texture*& pTexture, const SDL_Rect& pPosition, const SDL_Rect* pClip /*= nullptr*/, const Colour& pFilter /*= Colour::White*/, const double& pAngle /*= 0.0*/, const SDL_Point* pPivot /*= nullptr*/, const int& pFlip /*= 0*/) {
			//Check Colour filtering information
			if (pFilter.ID != ~0U && SDL_SetTextureColorMod(pTexture, pFilter.r, pFilter.g, pFilter.b)) 
				Globals::get<Debug::Logger>().logWarning("BasicSceneRenderer failed to apply the texture colour filter during a drawTexture(...) call. Error: %s", SDL_GetError());
			if (pFilter.a != 255U && SDL_SetTextureAlphaMod(pTexture, pFilter.a))
				Globals::get<Debug::Logger>().logWarning("BasicSceneRenderer failed to apply the texture alpha modification during a drawTexture(...) call. Error: %s", SDL_GetError());

			//Draw the texture
			if (SDL_RenderCopyEx(pRenderer, pTexture, pClip, &pPosition, pAngle, pPivot, (SDL_RendererFlip)pFlip))
				Globals::get<Debug::Logger>().logError("BasicSceneRenderer failed to render the Texture during a drawTexture(...) call. Error %s", SDL_GetError());

			//Reset filtering on Texture
			if (pFilter.ID != ~0U && SDL_SetTextureColorMod(pTexture, 255U, 255U, 255U))
				Globals::get<Debug::Logger>().logWarning("BasicSceneRenderer failed to reset the texture colour filter during a drawTexture(...) call. Error: %s", SDL_GetError());
			if (pFilter.a != 255U && SDL_SetTextureAlphaMod(pTexture, 255U))
				Globals::get<Debug::Logger>().logWarning("BasicSceneRenderer failed to reset the texture alpha modification during a drawTexture(...) call. Error: %s", SDL_GetError());
		}

		/*
			BasicSceneRenderer : drawLine - Render a line using passed in values
			Created: 09/10/2017
			Modified: 09/10/2017

			param[in] pRenderer - The SDL Renderer object to be used to draw graphics to the screen
			param[in] pFrom - An SDL_Point defining the starting position of the line
			param[in] pTo - An SDL_Point defining the end position of the line
			param[in] pColour - A Colour object defining the colour to draw the line in (Default Black)
		*/
		void BasicSceneRenderer::drawLine(SDL_Renderer*& pRenderer, const SDL_Point& pFrom, const SDL_Point& pTo, const Colour& pColour /*= Colour::Black*/) {
			//Set the Renderers draw colour
			if (SDL_SetRenderDrawColor(pRenderer, pColour.r, pColour.g, pColour.b, pColour.a))
				Globals::get<Debug::Logger>().logWarning("BasicSceneRenderer failed to apply the colour during a drawLine(...) call. Error: %s", SDL_GetError());

			//Render the line
			if (SDL_RenderDrawLine(pRenderer, pFrom.x, pFrom.y, pTo.x, pTo.y))
				Globals::get<Debug::Logger>().logError("BasicSceneRenderer failed to render the line during a drawLine(...) call. Error: %s", SDL_GetError());
		}

		/*
			BasicSceneRenderer : drawRect - Render a uniform area using passed in values
			Created: 09/10/2017
			Modified: 09/10/2017

			param[in] pRenderer - The SDL Renderer object to be used to draw graphics to the screen
			param[in] pPosition - An SDL_Rect object defining where the image is to be rendered
			param[in] pColour - A Colour object defining the colour to render the outline in or render the fill if pFill is true
			param[in] pFill - A boolean flag indicating if the Rectangle should be filled in (Default false)
			param[in] pBorderColour - A pointer to a Colour object to render the outline of the area if pFill is true (Default nullptr)
		*/
		void BasicSceneRenderer::drawRect(SDL_Renderer*& pRenderer, const SDL_Rect& pPosition, const Colour& pColour, const bool& pFill /*= false*/, const Colour* pBorderColour /*= nullptr*/) {
			//Check if the rectangle should be filled in
			if (pFill) drawTexture(pRenderer, mBlank, pPosition, nullptr, pColour);

			//Check if the rendering is finished
			if (pFill && !pBorderColour) return;

			//Store the colour to render on the outline
			Colour border = (pFill ? *pBorderColour : pColour);

			//Set the Renderers draw colour
			if (SDL_SetRenderDrawColor(pRenderer, border.r, border.g, border.b, border.a))
				Globals::get<Debug::Logger>().logWarning("BasicSceneRenderer failed to apply the colour during a drawRect(...) call. Error: %s", SDL_GetError());

			//Render the rectangle
			if (SDL_RenderDrawRect(pRenderer, &pPosition))
				Globals::get<Debug::Logger>().logError("BasicSceneRenderer failed to render the rectangle outline during a drawRect(...) call. Error: %s", SDL_GetError());
		}

		/*
			BasicSceneRenderer : drawText - Render text using the passed in values
			Created: 09/10/2017
			Modified: 09/10/2017

			param[in] pRenderer - The SDL Renderer object to be used to draw graphics to the screen
			param[in] pText - A c-string defining the text to render
			param[in] pFont - An SDL_Font to be used to render the Text
			param[in] pPosition - An SDL_Point defining where the text should be rendered
			param[in] pColour - A Colour object defining what colour the text should rendered in (Default Black)
			param[in] pAlignment - An ETextAlignment value defining if the text should be aligned a specific way (Default Null)
			param[in] pRenderType - An ETextRenderType value defining how the text should be rendered (Default Solid)
		*/
		void BasicSceneRenderer::drawText(SDL_Renderer*& pRenderer, const char*& pText, _TTF_Font*& pFont, const SDL_Point& pPosition, const Colour& pColour /*= Colour::Black*/, const ETextAlignment& pAlignment /*= ETextAlignment::Null*/, const ETextRenderType& pRenderType /*= ETextRenderType::Solid*/) {
			//Render the text to an SDL_Surface
			SDL_Surface* tempSurface = nullptr;

			//Switch on the render type
			switch (pRenderType) {
			case ETextRenderType::Blended:
				tempSurface = TTF_RenderText_Blended(pFont, pText, { pColour.r, pColour.g, pColour.b, pColour.a });
				break;
			default: // Solid
				tempSurface = TTF_RenderText_Solid(pFont, pText, { pColour.r, pColour.g, pColour.b, pColour.a });
				break;
			}

			//Check for error
			if (!tempSurface) {
				//Output error
				Globals::get<Debug::Logger>().logError("BasicSceneRenderer failed to render text '%s' to a surface with the type %i during a drawText(...) call. Error: %i", pText, (int)pRenderType, SDL_GetError());
				return;
			}

			//Convert the Surface to a Texture
			SDL_Texture* textTex = SDL_CreateTextureFromSurface(pRenderer, tempSurface);

			//Check for error
			if (!textTex) {
				//Output error
				Globals::get<Debug::Logger>().logError("BasicSceneRenderer failed to convert temporary SDL_Surface to a Texture during a drawText(...) call. Error: %s", SDL_GetError());

				//Free the temp surface
				SDL_FreeSurface(tempSurface);
				return;
			}

			//Free the temp surface
			SDL_FreeSurface(tempSurface);

			//Set the blending mode on the texture
			if (SDL_SetTextureBlendMode(textTex, SDL_BLENDMODE_BLEND))
				Globals::get<Debug::Logger>().logWarning("BasicSceneRenderer failed to set the Blend Mode of the texture containing the text '%s' during a drawText(...) call. Error: %s", pText, SDL_GetError());

			//Create a Rect to store the position and dimensions
			SDL_Rect pos = { pPosition.x, pPosition.y };

			//Get sizing information for the Texture
			if (SDL_QueryTexture(textTex, nullptr, nullptr, &pos.w, &pos.h)) {
				//Output error
				Globals::get<Debug::Logger>().logError("BasicSceneRenderer failed to get sizing information on the rendered string '%s' during a drawText(...) call. Error: %s", pText, SDL_GetError());

				//Destroy the texture
				SDL_DestroyTexture(textTex);
				return;
			}

			//Position the text
			switch (pAlignment) {
			case ETextAlignment::Center:
				pos.x += pos.w / 2;
				break;
			case ETextAlignment::Right:
				pos.x += pos.w;
				break;
			}

			//Check for transparency
			if (pColour.a != 255 && SDL_SetTextureAlphaMod(textTex, pColour.a))
				Globals::get<Debug::Logger>().logWarning("BasicSceneRenderer failed to apply the texture alpha modification during a drawText(...) call. Error: %s", SDL_GetError());

			//Draw the Text
			if (SDL_RenderCopyEx(pRenderer, textTex, nullptr, &pos, 0.0, nullptr, SDL_FLIP_NONE))
				Globals::get<Debug::Logger>().logError("BasicSceneRenderer failed to render the texture containing the text '%s' during a drawText(...) call. Error: %s", pText, SDL_GetError());

			//Destroy the texture
			SDL_DestroyTexture(textTex);
		}

		/*
			BasicSceneRenderer : Default Constructor - Initialise with default values
			Created: 09/10/2017
			Modified: 09/10/2017
		*/
		BasicSceneRenderer::BasicSceneRenderer() : mBlank(nullptr) {}

		/*
			BasicSceneRenderer : createRenderer - Setup the Scene Renderer
			Created: 09/10/2017
			Modified: 09/10/2017

			param[in] pRenderer - The SDL_Renderer object to allow for the creation of default textures

			return bool - Returns true if the Scene Renderer was created successfully
		*/
		bool BasicSceneRenderer::createRenderer(SDL_Renderer*& pRenderer) {
			//Setup RGBA masks for the endian order of the machine
			Uint32 rmask, gmask, bmask, amask;
			if (Utilities::Endian::getEndianOrder() == Utilities::Endian::EEndianOrder::Big) {
				rmask = 0xff000000;
				gmask = 0x00ff0000;
				bmask = 0x0000ff00;
				amask = 0x000000ff;
			} else {
				rmask = 0x000000ff;
				gmask = 0x0000ff00;
				bmask = 0x00ff0000;
				amask = 0xff000000;
			}

			//Setup the blank texture
			SDL_Surface* tempSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, 1, 1, 32, rmask, gmask, bmask, amask);

			//Check the surface was created successfully
			if (!tempSurface) {
				//Output the error
				Globals::get<Debug::Logger>().logError("BasicSceneRenderer failed to create a 1x1 RGBA Surface for the blank texture. Error: %s", SDL_GetError());

				//Return failure
				return false;
			}

			//Get the pixel data from the surface
			unsigned int* pixels = (unsigned int*)tempSurface->pixels;

			//Set as completely white so it can have a colour filter applied
			pixels[0] = ~0U;

			//Store the following process success state in a flag
			bool successFlag = false;

			//Convert the surface to a Texture
			mBlank = SDL_CreateTextureFromSurface(pRenderer, tempSurface);

			//Check the Texture was created successfully
			if (successFlag = !mBlank) 
				Globals::get<Debug::Logger>().logError("BasicSceneRenderer failed to convert the 1x1 RGBA surface to an SDL_Texture. Error: %s", SDL_GetError());

			//Set the blend mode for the blank texture
			else if (successFlag = (SDL_SetTextureBlendMode(mBlank, SDL_BLENDMODE_BLEND) != 0)) 
				Globals::get<Debug::Logger>().logError("BasicSceneRenderer failed to apply SDL_BLENDMODE_BLEND to the 1x1 RGBA Blank Texture. Error: %s", SDL_GetError());

			//Free the temp surface
			SDL_FreeSurface(tempSurface);

			//Return the success state
			return successFlag;
		}

		/*
			BasicSceneRenderer : destroyRenderer - Free memory and resources for the Scene Renderer
			Created: 09/10/2017
			Modified: 09/10/2017
		*/
		void BasicSceneRenderer::destroyRenderer() {
			//Check if the Blank Texture exists
			if (mBlank) {
				//Destroy the texture
				SDL_DestroyTexture(mBlank);
				mBlank = nullptr;
			}
		}
	}
}
