#include "Window.hpp"

//! Include the SDL2_Engine values
#include "WindowInitialiser.hpp"
#include "../Globals.hpp"
#include "../Debug/Logger.hpp"

//! Include the SDL objects
#include <SDL.h>

namespace SDL2_Engine {
	/*
	 *		Name: WindowInternalData
	 *		Author: Mitchell Croft
	 *		Created: 06/10/2017
	 *		Modified: 06/10/2017
	 *		
	 *		Purpose:
	 *		Store the internal data used by the Window
	**/
	struct Window::WindowInternalData {
		//! Dodgy method to save memory for two pointers
		union {
			//! Store a pointer to the SDL Window
			SDL_Window* window;

			//! Store a temporary pointer for the setup values
			const Initialisation::WindowInitialiser* setup;
		};
	};

	/*
		Window : setDimensions - Set the size of window
		Created: 06/10/2017
		Modified: 06/10/2017

		param[in] pDim - A Dimension2D object holding the new screen size values
	*/
	void Window::setDimensions(const Dimension2D& pDim) {
		SDL_SetWindowSize(mData->window, pDim.x, pDim.y);
		SDL_SetWindowPosition(mData->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}

	/*
		Window : setBordered - Assign or remove a border from the Window
		Created: 06/10/2017
		Modified: 06/10/2017

		param[in] pState - A boolean flag indicating assignment or removal
	*/
	void Window::setBordered(const bool& pState) { SDL_SetWindowBordered(mData->window, pState ? SDL_TRUE : SDL_FALSE); }

	/*
		Window : setFullScreen - Assign or remove fullscreen from the Window
		Created: 06/10/2017
		Modified: 06/10/2017

		param[in] pState - A boolean flag indicating the new state
	*/
	void Window::setFullScreen(const bool& pState) {
		//Toggle the fullscreen effect
		if (SDL_SetWindowFullscreen(mData->window, pState ? SDL_WINDOW_FULLSCREEN : NULL)) 
			Globals::get<Debug::Logger>().logError("Window failed to set the Full Screen state to '%s'. Error: %s", (pState ? "true" : "false"), SDL_GetError());
	}

	/*
		Window : minimise - Minimise the Window
		Created: 06/10/2017
		Modified: 06/10/2017
	*/
	void Window::minimise() { SDL_MinimizeWindow(mData->window); }

	/*
		Window : maximise - Maximise the Window
		Created: 06/10/2017
		Modified: 06/10/2017
	*/
	void Window::maximise() { SDL_MaximizeWindow(mData->window); }

	/*
		Window : show - Show the Window
		Created: 06/10/2017
		Modified: 06/10/2017
	*/
	void Window::show() { SDL_ShowWindow(mData->window); }

	/*
		Window : hide - Hide the Window
		Created: 06/10/2017
		Modified: 06/10/2017
	*/
	void Window::hide() { SDL_HideWindow(mData->window); }

	/*
		Window : getWindow - Retrieve a pointer to the SDL window object
		Created: 06/10/2017
		Modified: 06/10/2017

		return SDL_Window* - Returns a pointer to the internal SDL_Window object
	*/
	SDL_Window* Window::getWindow() { return mData->window;	}

	/*
		Window : getDimensions - Retrieve the current screen dimensions of the window
		Created: 06/10/2017
		Modified: 06/10/2017

		return Dimension2D - Returns a Dimension2D object holding the screen size values
	*/
	Dimension2D Window::getWindowDimensions() const {
		//Get the dimensions from the Window
		Dimension2D dim;
		SDL_GetWindowSize(mData->window, &dim.x, &dim.y);
		return dim;
	}

	/*
		Window : hasProperty - Determine if the current Window has the specified properties
		Created: 06/10/2017
		Modified: 06/10/2017

		param[in] pMask - A Bitmask of EWindowProperties to compare against the Window

		return bool - Returns true if the Window has all properties defined in pMask
	*/
	bool Window::hasProperty(const Utilities::Bitmask<EWindowProperties>& pMask) const {
		//Get the current flags for the window
		size_t flags = SDL_GetWindowFlags(mData->window);

		//Compare flags
		return ((pMask & flags) == pMask);
	}

	/*
		Window : Constructor - Initialise with default values
		Created: 06/10/2017
		Modified: 06/10/2017

		param[in] pSetup - Defines how the Window should be setup
	*/
	Window::Window(const Initialisation::WindowInitialiser& pSetup) : mData(nullptr) {
		//Create the data object
		mData = new WindowInternalData();

		//Stash the setup values
		mData->setup = &pSetup;
	}

	/*
		Window : createInterface - Allocate the resources required for the Keyboard object
		Created: 06/10/2017
		Modified: 06/10/2017

		return bool - Returns true if the Keyboard object was created successfully
	*/
	bool Window::createInterface() {
		//Load the Video subsystem for SDL
		if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
			//Output error
			Globals::get<Debug::Logger>().logError("Window failed to initialise SDL's Video subsystem. Error: %s", SDL_GetError());
			return false;
		}

		//Store the pointer to the setup values
		const Initialisation::WindowInitialiser* setup = mData->setup;

		//Create the SDL Window
		mData->window = SDL_CreateWindow(setup->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, setup->width, setup->height, setup->properties.getMask());

		//Check the window loaded correctly
		if (!mData->window) {
			//Output error
			Globals::get<Debug::Logger>().logError("Window failed to create the SDL Window object. Error: %s", SDL_GetError());
			return false;
		}

		//Return success
		return true;
	}

	/*
		Window : destroyInterface - Deallocate the resources used by the Keyboard object
		Created: 06/10/2017
		Modified: 06/10/2017
	*/
	void Window::destroyInterface() {
		//Ensure there is data to remove
		if (mData) {
			//Destroy the window
			if (mData->window) SDL_DestroyWindow(mData->window);

			//Close the SDL Video subsystem
			SDL_QuitSubSystem(SDL_INIT_VIDEO);

			//Delete the internal data object
			delete mData;
			mData = nullptr;
		}
	}
}