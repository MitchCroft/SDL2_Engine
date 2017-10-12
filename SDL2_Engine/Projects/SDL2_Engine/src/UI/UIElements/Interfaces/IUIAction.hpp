#pragma once

//! Include the SDL2_Engine objects
#include "../../../__LibraryManagement.hpp"
#include "../../../Utilities/Action.hpp"

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			//! Prototype the IUIAction interface
			class IUIAction;

			//! Create a type define for UI Actions 
			typedef Utilities::Action<void(IUIAction*, void*)> UIAction;

			/*
			 *		Name: IUIAction
			 *		Author: Mitchell Croft
			 *		Created: 11/10/2017
			 *		Modified: 11/10/2017
			 *		
			 *		Purpose:
			 *		Provide an interface for UI elements that can perform an action in response to
			 *		user input
			**/
			class SDL2_LIB_INC IUIAction {
			public:
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////--------------------------------Interface Operators----------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				
				/*
					IUIAction : action - Call the contained UI action object
					Created: 11/10/2017
					Modified: 11/10/2017

					return bool - Returns true if a callback was raised
				*/
				inline bool action() { return mActionCallback(this, mActionData); }

				/*
					IUIAction : setAction - Set the callback function used for the action
					Created: 11/10/2017
					Modified: 11/10/2017

					param[in] pCB - A pointer to the new callback function that should be used for the action
				*/
				inline void setAction(const UIAction::signature& pCB) { mActionCallback = pCB; }

				/*
					IUIAction : setData - Set the data pointer that is passed to the action callback
					Created: 11/10/2017
					Modified: 11/10/2017

					param[in] pData - A pointer to the generic data to be used to process action results
				*/
				inline void setData(void* pData) { mActionData = pData; }

				/*
					IUIAction : Constructor - Initialise with default values
					Created: 11/10/2017
					Modified: 11/10/2017
				*/
				inline IUIAction() : mActionCallback(nullptr), mActionData(nullptr) {}

			private:
				//! Store a UIAction object
				UIAction mActionCallback;

				//! Store the data pointer to pass to the action
				void* mActionData;
			};
		}
	}
}