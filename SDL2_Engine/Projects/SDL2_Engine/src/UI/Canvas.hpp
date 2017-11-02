#pragma once

//! Include SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "../Utilities/IGlobal.hpp"
#include "UIElements/UIValues.hpp"

//! Prototype the Objx Node
namespace Objx { class Node; }

namespace SDL2_Engine {
	//! Prototype the Canvas Initialiser
	namespace Initialisation { struct CanvasInitialiser; }

	namespace UI {
		//! Prototype the UI interface
		namespace UIElements { class IUIBase; class IUIAction; }

		/*
		 *		Name: Canvas
		 *		Author: Mitchell Croft
		 *		Created: 11/10/2017
		 *		Modified: 02/11/2017
		 *		
		 *		Purpose:
		 *		Provide a uniform interface for managing the navigation, operation and displaying 
		 *		of UI elements
		**/
		class SDL2_LIB_INC Canvas : public Utilities::IGlobal {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------------UI Management----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Canvas : createUI - Create a new UI element of a specified type and return it for setting up
				Created: 13/10/2017
				Modified: 13/10/2017

				Template T - The type of UI element to create
				Template TArgs - A parameter pack of template types used to setup the new UI element

				param[in] pArgs - A parameter pack of values used to setup the new UI element

				return T* - Returns a pointer to an object of type T or nullptr if unable to create

				Note:
				If the UI element created is an IUIAction derived class, rebuildInteractionMap will need to be called before it 
				can be accessed by user input
			*/
			template<typename T, typename ... TArgs>
			T* createUI(TArgs ... pArgs) {
				//Ensure the type is valid
				static_assert(std::is_base_of<UIElements::IUIBase, T>::value, "Can not create a UI element that is not a base of IUIBase");

				//Attempt to stash the new element
				return (T*)stashUI(new T(pArgs...));
			}

			/*
				Canvas : getUI - Get the first UI element with the specified tag
				Created: 13/10/2017
				Modified: 13/10/2017

				param[in] pTag - The tag string to look for

				return UIElements::IUIBase* - Returns a pointer to the first UIElement with the specified tag or nullptr if not found
			*/
			UIElements::IUIBase* getUI(const uiTag& pTag);

			/*
				Canvas : setActionObject - Set the active interactive object
				Created: 13/10/2017
				Modified: 13/10/2017

				param[in] pObj - A pointer to the IUIAction object to make active

				return bool - Returns true if the supplied object was found and selected
			*/
			bool setActionObject(const UIElements::IUIAction* pObj);

			/*
				Canvas : setActionObject - Set the active interactive object base on a tag
				Created: 13/10/2017
				Modified: 13/10/2017

				param[in] pTag - The tag string to look for

				return bool - Returns true if a matching IUIAction was found
			*/
			bool setActionObject(const uiTag& pTag);

			/*
				Canvas : destroyUI - Destroy the first UI element with the specified tag
				Created: 13/10/2017
				Modified: 13/10/2017

				param[in] pTag - The tag string to look for

				return bool - Returns true if a UI element was found and removed

				Note:
				If the UI element destroyed is an IUIAction derived class, the interaction map will become invalidated and will require
				a call to rebuildInteractionMap before user input will work
			*/
			bool destroyUI(const uiTag& pTag);

			/*
				Canvas : destroyUI - Destroy the first UI element of the specified type
				Created: 13/10/2017
				Modified: 13/10/2017

				param[in] pType - An EUIType value defining the type of UI element to destroy

				return bool - Returns true if a UI element was found and removed

				Note:
				If the UI element destroyed is an IUIAction derived class, the interaction map will become invalidated and will require
				a call to rebuildInteractionMap before user input will work
			*/
			bool destroyUI(const EUIType& pType);

			/*
				Canvas : clearCanvas - Clear the Canvas of all UI elements with a specific tag
				Created: 13/10/2017
				Modified: 13/10/2017

				param[in] pTag - The tag string to look for

				return bool - Returns true if a UI element was found and removed

				Note:
				If a UI element destroyed is an IUIAction derived class, the interaction map will become invalidated and will require
				a call to rebuildInteractionMap before user input will work
			*/
			bool clearCanvas(const uiTag& pTag);

			/*
				Canvas : clearCanvas - Clear the Canvas of all UI elements of a specific type
				Created: 13/10/2017
				Modified: 13/10/2017

				param[in] pType - An EUIType value defining the type of UI element to destroy

				return bool - Returns true if a UI element was found and removed

				Note:
				If a UI element destroyed is an IUIAction derived class, the interaction map will become invalidated and will require
				a call to rebuildInteractionMap before user input will work
			*/
			bool clearCanvas(const EUIType& pType);

			/*
				Canvas : clearCanvas - Remove all UI elements from the Canvas
				Created: 13/10/2017
				Modified: 13/10/2017
			*/
			void clearCanvas();

			/*
				Canvas : loadCanvasFromObjx - Load a collection of UI elements from an Objx file
				Created: 13/10/2017
				Modified: 13/10/2017

				param[in] pFilepath - The filepath of the UI Objx descriptor
				param[in] pName - The optional name of an Objx element within the Objx file to use as the Canvas (Default nullptr uses all elements in the root of the document)

				return bool - Returns true if it was able to load elements from the file
			*/
			bool loadCanvasFromObjx(const char* pFilepath, const char* pName = nullptr);

			/*
				Canvas : rebuildInteractionMap - Rebuild the internal interaction map to allow for users to navigate Actionable UI elements
				Created: 13/10/2017
				Modified: 02/11/2017
			*/
			void rebuildInteractionMap();


			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Construction/Destruction-----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Canvas : Constructor - Initialise with default values
				Created: 12/10/2017
				Modified: 13/10/2017

				param[in] pSetup - Defines how the Canvas should be setup
			*/
			Canvas(const Initialisation::CanvasInitialiser& pSetup);

			/*
				Canvas : createInterface - Verify and setup starting information
				Created: 12/10/2017
				Modified: 16/10/2017

				return bool - Returns true if the Canvas was setup correctly
			*/
			bool createInterface() override;

			/*
				Canvas : destroyInterface - Deallocate internal memory allocated
				Created: 12/10/2017
				Modified: 13/10/2017
			*/
			void destroyInterface() override;

			/*
				Canvas : update - Update and render the contained UI elements
				Created: 12/10/2017
				Modified: 02/11/2017
			*/
			void update() override;

		private:
			//! Define the internal protected elements for the Canvas
			struct CanvasInternalData;
			CanvasInternalData* mData;

			//! Stash template created functions in the Canvas
			UIElements::IUIBase* stashUI(UIElements::IUIBase* pNew);

			//! Load a UI element from an Objx Node
			void loadUIElementFromObjx(const char* pOrigin, const char* pName, const Objx::Node& pNode);

			//! Update the Action UI elements
			void updateActionUI();

			//! Destroy the current interactive map
			void destroyInteractiveMap();
		};
	}
}