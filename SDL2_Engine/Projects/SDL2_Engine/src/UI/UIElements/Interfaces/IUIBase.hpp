#pragma once

//! Include the SDL2_Engine objects
#include "UIBounds.hpp"
#include "../UIValues.hpp"

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			/*
			 *		Name: IUIBase
			 *		Author: Mitchell Croft
			 *		Created: 12/10/2017
			 *		Modified: 13/10/2017
			 *		
			 *		Purpose:
			 *		Provide an interface for UI elements to implement from
			 *		
			 *		Notes:
			 *		This UI component can be initialised by default through the Canvas.loadCanvasFromObjx function.
			 *		Settable values are:
			 *		
			 *		type - A string defining the type of object to create (Default types are "panel", "label", "button", "textbox")
			 *		tag - A string value that can be used to differentiate and identify the UI element
			 *		x - An integer value defining the X coordinate of the UI element
			 *		y - An integer value defining the Y coordinate of the UI element
			 *		width - An integer value defining the width of the UI element
			 *		height - An integer value defining the height of the UI element
			**/
			class SDL2_LIB_INC IUIBase {
			public:
				//! Keep a constant value that identifies the type of UI object the inheriting is
				const EUIType type;

				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////-------------------------------Implementable Interface-------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				/*
					IUIBase : Constructor - Initialise with default values
					Created: 12/10/2017
					Modified: 13/10/2017

					param[in] pType - An EUIType value defining the type of UI element this object is
				*/
				inline IUIBase(const EUIType& pType) : type(pType), mAlive(true) {}

				/*
					IUIBase : Destructor - Override-able destructor to allow for memory cleanup
					Created: 12/10/2017
					Modified: 12/10/2017
				*/
				inline virtual ~IUIBase() {}

				/*
					IUIBase : createUI - Function to facilitate the setup and validation of UI element
					Created: 12/10/2017
					Modified: 13/10/2017

					return bool - Returns true if the UI element was created successfully
				*/
				virtual bool createUI() { return true; }

				/*
					IUIBase : destroyUI - Function to facilitate the deallocation of resources for a UI element
					Created: 12/10/2017
					Modified: 13/10/2017
				*/
				virtual void destroyUI() {}

				/*
					IUIBase : update - Function to facilitate the updating of values once per cycle
					Created: 12/10/2017
					Modified: 12/10/2017
				*/
				inline virtual void update() {}

				/*
					IUIBase : render - Function to facilitate the the rendering of images once per cycle
					Created: 12/10/2017
					Modified: 12/10/2017
				*/
				virtual void render() = 0;

				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////-----------------------------------Data Accessors------------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				
				/*
					IUIBase : getTag - Get the information tagged to the UI element
					Created: 12/10/2017
					Modified: 13/10/2017

					return const uiTag& - Returns a constant uiTag reference holding the tag data
				*/
				inline const uiTag& getTag() const { return mTag; }

				/*
					IUIBase : setTag - Set the tag information associated with the UI element
					Created: 12/10/2017
					Modified: 13/10/2017

					param[in] pTag - A pointer to a string of characters to be used as the new tag information
				*/
				inline void setTag(const char* pTag) { mTag = pTag; }

				/*
					IUIBase : getLocation - Get the location values for the UI element
					Created: 13/10/2017
					Modified: 13/10/2017

					return const UIBounds& - Returns a constant reference to the internal location object
				*/
				inline const UIBounds& getLocation() const { return mLocation; }

				/*
					IUIBase : setLocation - Set the location values for the UI element
					Created: 13/10/2017
					Modified: 13/10/2017

					param[in] pLoc - A UIBounds object defining the new location
				*/
				inline void setLocation(const UIBounds& pLoc) { mLocation = pLoc; }

				/*
					IUIBase : isAlive - Get the alive flag of the current UI element
					Created: 13/10/2017
					Modified: 13/10/2017

					return const bool& - Returns a constant reference to the internal flag
				*/
				inline const bool& isAlive() const { return mAlive; }

				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////------------------------------------Main Function------------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				/*
					IUIBase : kill - Flag the current UI element for removal 
					Created: 13/10/2017
					Modified: 13/10/2017
				*/
				inline void kill() { mAlive = false; }

			protected:
				//! Store tag information for the UI element
				uiTag mTag;

				//! Store the position of the UI element
				UIBounds mLocation;

			private:
				//! Flag if the UI element is still alive
				bool mAlive;
			};
		}
	}
}