#pragma once

//! Include the SDL2_Engine objects
#include "../../../__LibraryManagement.hpp"
#include "../../UIValues.hpp"

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			/*
			 *		Name: IUIBase
			 *		Author: Mitchell Croft
			 *		Created: 12/10/2017
			 *		Modified: 12/10/2017
			 *		
			 *		Purpose:
			 *		Provide an interface for UI elements to implement from
			**/
			class SDL2_LIB_INC IUIBase {
			public:
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////-------------------------------Implementable Interface-------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				/*
					IUIBase : Constructor - Initialise with default values
					Created: 12/10/2017
					Modified: 12/10/2017
				*/
				IUIBase();

				/*
					IUIBase : Destructor - Override-able destructor to allow for memory cleanup
					Created: 12/10/2017
					Modified: 12/10/2017
				*/
				inline virtual ~IUIBase() {}

				/*
					IUIBase : createUI - Function to facilitate the setup and validation of UI element
					Created: 12/10/2017
					Modified: 12/10/2017

					return bool - Returns true if the UI element was created successfully
				*/
				virtual bool createUI() = 0;

				/*
					IUIBase : destroyUI - Function to facilitate the deallocation of resources for a UI element
					Created: 12/10/2017
					Modified: 12/10/2017
				*/
				virtual void destroyUI() = 0;

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
					Modified: 12/10/2017

					return const cstring& - Returns a constant cstring reference holding the tag data
				*/
				inline const cstring& getTag() const { return mTag; }

				/*
					IUIBase : setTag - Set the tag information associated with the UI element
					Created: 12/10/2017
					Modified: 12/10/2017

					param[in] pTag - A pointer to a string of characters to be used as the new tag information
				*/
				void setTag(const char* pTag);

			protected:
				//! Store tag information for the UI element
				cstring mTag;
			};
		}
	}
}