#pragma once

//! Include the required SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "../Utilities/IGlobal.hpp"
#include "../Utilities/Bitmask.hpp"
#include "WindowValues.hpp"

//! Prototype the SDL Window object
struct SDL_Window;

namespace SDL2_Engine {
	//! Prototype the initialiser
	namespace Initialisation { struct WindowInitialiser; }

	/*
	 *		Name: Window
	 *		Author: Mitchell Croft
	 *		Created: 06/10/2017
	 *		Modified: 06/10/2017
	 *		
	 *		Purpose:
	 *		Manage functionality relating to the creation and management of the SDL window
	**/
	class SDL2_LIB_INC Window : public Utilities::IGlobal {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------------Modifiers------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Window : setDimensions - Set the size of window
			Created: 06/10/2017
			Modified: 06/10/2017

			param[in] pDim - A Dimension2D object holding the new screen size values
		*/
		void setDimensions(const Dimension2D& pDim);

		/*
			Window : setBordered - Assign or remove a border from the Window
			Created: 06/10/2017
			Modified: 06/10/2017

			param[in] pState - A boolean flag indicating assignment or removal
		*/
		void setBordered(const bool& pState);

		/*
			Window : setFullScreen - Assign or remove fullscreen from the Window
			Created: 06/10/2017
			Modified: 06/10/2017

			param[in] pState - A boolean flag indicating the new state
		*/
		void setFullScreen(const bool& pState);

		/*
			Window : minimise - Minimise the Window
			Created: 06/10/2017
			Modified: 06/10/2017
		*/
		void minimise();

		/*
			Window : maximise - Maximise the Window
			Created: 06/10/2017
			Modified: 06/10/2017
		*/
		void maximise();

		/*
			Window : show - Show the Window
			Created: 06/10/2017
			Modified: 06/10/2017
		*/
		void show();

		/*
			Window : hide - Hide the Window
			Created: 06/10/2017
			Modified: 06/10/2017
		*/
		void hide();

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-------------------------------------------Data--------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Window : getWindow - Retrieve a pointer to the SDL window object
			Created: 06/10/2017
			Modified: 06/10/2017

			return SDL_Window* - Returns a pointer to the internal SDL_Window object
		*/
		SDL_Window* getWindow();

		/*
			Window : getDimensions - Retrieve the current screen dimensions of the window
			Created: 06/10/2017
			Modified: 06/10/2017

			return Dimension2D - Returns a Dimension2D object holding the screen size values
		*/
		Dimension2D getWindowDimensions() const;

		/*
			Window : hasProperty - Determine if the current Window has the specified properties
			Created: 06/10/2017
			Modified: 06/10/2017

			param[in] pMask - A Bitmask of EWindowProperties to compare against the Window

			return bool - Returns true if the Window has all properties defined in pMask
		*/
		bool hasProperty(const Utilities::Bitmask<EWindowProperties>& pMask) const;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------Construction/Destruction-----------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/*
			Window : Constructor - Initialise with default values
			Created: 06/10/2017
			Modified: 06/10/2017

			param[in] pSetup - Defines how the Window should be setup
		*/
		Window(const Initialisation::WindowInitialiser& pSetup);

		/*
			Window : createInterface - Allocate the resources required for the Window object
			Created: 06/10/2017
			Modified: 06/10/2017

			return bool - Returns true if the Window object was created successfully
		*/
		bool createInterface() override;

		/*
			Window : destroyInterface - Deallocate the resources used by the Window object
			Created: 06/10/2017
			Modified: 06/10/2017
		*/
		void destroyInterface() override;

		/*
			Window : update - Empty function
			Created: 06/10/2017
			Modified: 06/10/2017
		*/
		inline void update() override {}

	private:
		//! Define the internal protected elements for the Window
		struct WindowInternalData;
		WindowInternalData* mData;
	};
}