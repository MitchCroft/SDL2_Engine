#include "Renderer.hpp"

//! Include the SDL2_Engine values
#include "RendererInitialiser.hpp"
#include "../Globals.hpp"
#include "../Debug/Logger.hpp"

//! Include the SDL objects
#include <SDL.h>
#include <SDL_ttf.h>

namespace SDL2_Engine {
	namespace Rendering {
		/*
		 *		Name: RendererInternalData
		 *		Author: Mitchell Croft
		 *		Created: 09/10/2017
		 *		Modified: 09/10/2017
		 *		
		 *		Purpose:
		 *		Store the internal data used by the Renderer
		**/
		struct Renderer::RendererInternalData {
			//! Dodgy method to save memory used for two pointers
			union {
				//! Save a pointer to the setup information being used
				const Initialisation::RendererInitialiser* setup;

				//! Store a pointer to the SDL_Renderer object
				SDL_Renderer* renderer = nullptr;
			};
			union {
				//! Save a pointer to the SDL_Window object
				SDL_Window* window;

				//! Store a pointer to the current SceneRenderer
				ISceneRenderer* sceneRenderer = nullptr;
			};
		};

		/*
			Renderer : drawTexture - Render a texture using passed in values
			Created: 09/10/2017
			Modified: 09/10/2017

			param[in] pTexture - A pointer to the SDL_Texture object to render
			param[in] pPosition - An SDL_Rect object defining where the image is to be rendered
			param[in] pClip - A pointer to an SDL_Rect defining a section of the texture to render (Default nullptr)
			param[in] pFilter - A Colour object defining the filter to be applied to the rendered image (Default White)
			param[in] pAngle - A double value defining the rotation amount in degrees (Default 0)
			param[in] pPivot - A pointer to an SDL_Point object defining the pivot point for the rotation (Default nullptr)
			param[in] pFlip - The SDL_RendererFlip value defining if the image should be flipped in any way (Default SDL_FLIP_NONE)
		*/
		void Renderer::drawTexture(SDL_Texture* pTexture, const SDL_Rect& pPosition, const SDL_Rect* pClip /*= nullptr*/, const Colour& pFilter /*= Colour::White*/, const double& pAngle /*= 0.0*/, const SDL_Point* pPivot /*= nullptr*/, const int& pFlip /*= 0*/) {
			mData->sceneRenderer->drawTexture(mData->renderer, pTexture, pPosition, pClip, pFilter, pAngle, pPivot, pFlip);
		}

		/*
			Renderer : drawLine - Render a line using passed in values
			Created: 09/10/2017
			Modified: 09/10/2017

			param[in] pFrom - An SDL_Point defining the starting position of the line
			param[in] pTo - An SDL_Point defining the end position of the line
			param[in] pColour - A Colour object defining the colour to draw the line in (Default Black)
		*/
		void Renderer::drawLine(const SDL_Point& pFrom, const SDL_Point& pTo, const Colour& pColour /*= Colour::Black*/) {
			mData->sceneRenderer->drawLine(mData->renderer, pFrom, pTo, pColour);
		}

		/*
			Renderer : drawRect - Render a uniform area using passed in values
			Created: 09/10/2017
			Modified: 09/10/2017

			param[in] pPosition - An SDL_Rect object defining where the image is to be rendered
			param[in] pColour - A Colour object defining the colour to render the outline in or render the fill if pFill is true
			param[in] pFill - A boolean flag indicating if the Rectangle should be filled in (Default false)
			param[in] pBorderColour - A pointer to a Colour object to render the outline of the area if pFill is true (Default nullptr)
		*/
		void Renderer::drawRect(const SDL_Rect& pPosition, const Colour& pColour, const bool& pFill /*= false*/, const Colour* pBorderColour /*= nullptr*/) {
			mData->sceneRenderer->drawRect(mData->renderer, pPosition, pColour, pFill, pBorderColour);
		}

		/*
			Renderer : drawText - Render text using the passed in values
			Created: 09/10/2017
			Modified: 09/10/2017

			param[in] pText - A c-string defining the text to render
			param[in] pFont - An SDL_Font to be used to render the Text
			param[in] pPosition - An SDL_Point defining where the text should be rendered
			param[in] pColour - A Colour object defining what colour the text should rendered in (Default Black)
			param[in] pAlignment - An ETextAlignment value defining if the text should be aligned a specific way (Default Null)
			param[in] pRenderType - An ETextRenderType value defining how the text should be rendered (Default Solid)
		*/
		void Renderer::drawText(const char* pText, _TTF_Font* pFont, const SDL_Rect& pPosition, const Colour& pColour /*= Colour::Black*/, const ETextAlignment& pAlignment /*= ETextAlignment::Null*/, const ETextRenderType& pRenderType /*= ETextRenderType::Solid*/) {
			mData->sceneRenderer->drawText(mData->renderer, pText, pFont, pPosition, pColour, pAlignment, pRenderType);
		}

		/*
			Renderer : getRenderer - Retrieve the SDL_Renderer object
			Created: 09/10/2017
			Modified: 09/10/2017

			return SDL_Renderer* - Returns a pointer to the SDL_Renderer object
		*/
		SDL_Renderer* Renderer::getRenderer() { return mData->renderer; }

		/*
			Renderer : Constructor - Initialise with default values
			Created: 05/10/2017
			Modified: 05/10/2017

			param[in] pWindow - A pointer to the SDL_Window object
			param[in] pSetup - Defines how the Renderer should be setup
		*/
		Renderer::Renderer(SDL_Window* pWindow, const Initialisation::RendererInitialiser& pSetup) : mData(nullptr) {
			//Create the internal data object
			mData = new RendererInternalData();

			//Stow the setup values
			mData->setup = &pSetup;
			mData->window = pWindow;
		}

		/*
			Renderer : createInterface - Verify and setup starting information
			Created: 05/10/2017
			Modified: 05/10/2017

			return bool - Returns true if the Resources Manager was setup correctly
		*/
		bool Renderer::createInterface() {
			//Keep temporary pointers to the stowed information
			const Initialisation::RendererInitialiser* setup = mData->setup;
			SDL_Window* window = mData->window;

			//Initialise TTF
			if (TTF_Init()) {
				//Output error
				Globals::get<Debug::Logger>().logError("Renderer failed to initialise the SDL_TTF library. Error: %s", SDL_GetError());

				//Reset the data pointers
				mData->renderer = nullptr;
				mData->sceneRenderer = nullptr;

				//Delete the assigned Scene Renderer (if it exists)
				if (setup->sceneRenderer) delete setup->sceneRenderer;

				//Return failure
				return false;
			}

			//Save the Scene Renderer pointer
			mData->sceneRenderer = setup->sceneRenderer;

			//Create the SDL_Renderer
			mData->renderer = SDL_CreateRenderer(window, setup->deviceIndex, setup->properties.getMask());

			//Check the Renderer was created 
			if (!mData->renderer) {
				//Output error
				Globals::get<Debug::Logger>().logError("Renderer failed to create the SDL_Renderer object. Error: %s", SDL_GetError());

				//Delete the assigned Scene Renderer (if it exists)
				if (mData->sceneRenderer) {
					delete mData->sceneRenderer;
					mData->sceneRenderer = nullptr;
				}

				//Return failure
				return false;
			}

			//Check if there is a Scene Renderer to setup
			if (mData->sceneRenderer) {
				//Check if creation failed
				if (!mData->sceneRenderer->createRenderer(mData->renderer)) {
					//Output the error
					Globals::get<Debug::Logger>().logError("Renderer failed to create the initial Scene Renderer object. Scene Renderer createRenderer() failed");

					//Clear up what it can
					mData->sceneRenderer->destroyRenderer();

					//Delete the Scene Renderer
					delete mData->sceneRenderer;

					//Reset pointer
					mData->sceneRenderer = nullptr;

					//Return failure
					return false;
				}
			}

			//Default return success
			return true;
		}

		/*
			Renderer : destroyInterface - Deallocate internal memory allocated
			Created: 05/10/2017
			Modified: 05/10/2017
		*/
		void Renderer::destroyInterface() {
			//Check there is data to operate on
			if (mData) {
				//Check if there is Scene Render to remove
				if (mData->sceneRenderer) {
					//Destroy the Scene Renderer
					mData->sceneRenderer->destroyRenderer();

					//Delete the Scene Renderer
					delete mData->sceneRenderer;
				}

				//Check if there is SDL_Renderer to destroy
				if (mData->renderer) SDL_DestroyRenderer(mData->renderer);

				//Deinitialise TTF
				TTF_Quit();

				//Delete the internal data
				delete mData;
				mData = nullptr;
			}
		}

		/*
			Renderer : returnSceneRenderer - Return the current Scene Renderer object
			Created: 09/10/2017
			Modified: 09/102/2017

			return ISceneRenderer* - Returns a pointer to the current ISceneRenderer object
		*/
		ISceneRenderer* Renderer::returnSceneRenderer() { return mData->sceneRenderer; }

		/*
			Renderer : setSceneRenderer - Set a new Scene Renderer to be used
			Created: 09/10/2017
			Modified: 09/10/2017

			param[in] pRenderer - A pointer to a new ISceneRenderer object

			return bool - Returns true if the new Scene Renderer was setup and set correctly
		*/
		bool Renderer::setSceneRenderer(ISceneRenderer* pRenderer) {
			//Attempt to setup the new Scene Renderer
			if (pRenderer->createRenderer(mData->renderer)) {
				//Destroy the old Scene Renderer
				mData->sceneRenderer->destroyRenderer();

				//Delete the old Scene Renderer
				delete mData->sceneRenderer;

				//Assign new Render object
				mData->sceneRenderer = pRenderer;

				//Return successful
				return true;
			}

			//Otherwise report failure
			else {
				//Output error message
				Globals::get<Debug::Logger>().logError("Renderer failed to create a new Scene Renderer object. Scene Renderer createRenderer() failed.");

				//Clear up what it can
				pRenderer->destroyRenderer();

				//Delete the failed Renderer
				delete pRenderer;

				//Return failure
				return false;
			}
		}
	}
}
