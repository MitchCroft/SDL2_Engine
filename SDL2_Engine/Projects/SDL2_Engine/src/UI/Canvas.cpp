#include "Canvas.hpp"

//! Include the SDL2_Engine objects
#include "UIElements/Interfaces/IUIBase.hpp"
#include "UIElements/Interfaces/IUIAction.hpp"

namespace SDL2_Engine {
	namespace UI {
		/*
		 *		Name: InteractiveUINode
		 *		Author: Mitchell Croft
		 *		Created: 11/10/2017
		 *		Modified: 11/10/2017
		 *		
		 *		Purpose:
		 *		Manage a network of UI elements that can be interacted with by the user
		**/
		struct InteractiveUINode {
			//! Store the cardinal directions 
			InteractiveUINode* top		= nullptr;
			InteractiveUINode* bottom	= nullptr;
			InteractiveUINode* left		= nullptr;
			InteractiveUINode* right	= nullptr;

			//! Store a pointer to current interactive element
			UIElements::IUIAction* element = nullptr;
		};

		/*
		 *		Name: CanvasInternalData
		 *		Author: Mitchell Croft
		 *		Created: 12/10/2017
		 *		Modified: 12/10/2017
		 *		
		 *		Purpose:
		 *		Store internal Canvas data values
		**/
		struct Canvas::CanvasInternalData {

		};

		/*
			Canvas : Constructor - Initialise with default values
			Created: 12/10/2017
			Modified: 12/10/2017
		*/
		Canvas::Canvas() {}

		/*
			Canvas : createInterface - Verify and setup starting information
			Created: 12/10/2017
			Modified: 12/10/2017

			return bool - Returns true if the Canvas was setup correctly
		*/
		bool Canvas::createInterface() {
			return true;
		}

		/*
			Canvas : destroyInterface - Deallocate internal memory allocated
			Created: 12/10/2017
			Modified: 12/10/2017
		*/
		void Canvas::destroyInterface() {

		}

		/*
			Canvas : update - Update and render the contained UI elements
			Created: 12/10/2017
			Modified: 12/10/2017
		*/
		void Canvas::update() {

		}
	}
}
