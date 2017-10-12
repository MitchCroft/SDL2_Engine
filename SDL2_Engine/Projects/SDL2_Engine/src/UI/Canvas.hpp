#pragma once

//! Include SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "../Utilities/IGlobal.hpp"

namespace SDL2_Engine {
	namespace UI {
		/*
		 *		Name: Canvas
		 *		Author: Mitchell Croft
		 *		Created: 11/10/2017
		 *		Modified: 11/10/2017
		 *		
		 *		Purpose:
		 *		Provide a uniform interface for managing the navigation, operation and displaying 
		 *		of UI elements
		**/
		class SDL2_LIB_INC Canvas : public Utilities::IGlobal {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Construction/Destruction-----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Canvas : Constructor - Initialise with default values
				Created: 12/10/2017
				Modified: 12/10/2017
			*/
			Canvas();

			/*
				Canvas : createInterface - Verify and setup starting information
				Created: 12/10/2017
				Modified: 12/10/2017

				return bool - Returns true if the Canvas was setup correctly
			*/
			bool createInterface() override;

			/*
				Canvas : destroyInterface - Deallocate internal memory allocated
				Created: 12/10/2017
				Modified: 12/10/2017
			*/
			void destroyInterface() override;

			/*
				Canvas : update - Update and render the contained UI elements
				Created: 12/10/2017
				Modified: 12/10/2017
			*/
			void update() override;

		private:
			//! Define the internal protected elements for the Canvas
			struct CanvasInternalData;
			CanvasInternalData* mData;
		};
	}
}