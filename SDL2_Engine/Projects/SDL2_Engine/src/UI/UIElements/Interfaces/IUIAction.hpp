#pragma once

//! Include the SDL2_Engine objects
#include "../../../__LibraryManagement.hpp"
#include <functional>

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			//! Prototype the IUIAction interface
			class IUIAction;

			//! Export the required std::function implementation
			template class SDL2_LIB_INC std::function<void(IUIAction*, void*)>;

			//! Create a type define for UI Actions 
			typedef std::function<void(IUIAction*, void*)> UIAction;

			//! Define the possible states that the action can exist in
			enum class EActionState { Locked, Default, Highlighted };

			/*
			 *		Name: IUIAction
			 *		Author: Mitchell Croft
			 *		Created: 11/10/2017
			 *		Modified: 02/11/2017
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
					Modified: 02/11/2017

					return bool - Returns true if a callback was raised
				*/
				inline bool action() { if (!mActionCallback) return false; mActionCallback(this, mActionData);	return true; }

				/*
					IUIAction : setAction - Set the callback function used for the action
					Created: 11/10/2017
					Modified: 02/11/2017

					param[in] pCB - A callback function object to be raised when the action is called
				*/
				inline void setAction(const UIAction& pCB) { mActionCallback = pCB; }

				/*
					IUIAction : setData - Set the data pointer that is passed to the action callback
					Created: 11/10/2017
					Modified: 11/10/2017

					param[in] pData - A pointer to the generic data to be used to process action results
				*/
				inline void setData(void* pData) { mActionData = pData; }

				/*
					IUIAction : getState - Get the current objects state
					Created: 13/10/2017
					Modified: 13/10/2017

					return const EActionState& - Returns a constant reference to the internal state value
				*/
				inline const EActionState& getState() const { return mState; }

				/*
					IUIAction : setState - Set the state of the current object
					Created: 13/10/2017
					Modified: 13/10/2017

					param[in] pState - An EActionState value representing the objects new state
				*/
				inline void setState(const EActionState& pState) { mState = pState; }

				/*
					IUIAction : Constructor - Initialise with default values
					Created: 11/10/2017
					Modified: 11/10/2017
				*/
				inline IUIAction() : mActionCallback(nullptr), mActionData(nullptr), mState(EActionState::Default) {}

				/*
					IUIAction : DEstructor - Allow for the deallocation of memory
					Created: 14/10/2017
					Modified: 14/10/2017
				*/
				virtual ~IUIAction() {}

			protected:
				//! Store the current state of the Object
				EActionState mState;

			private:
				//! Store a UIAction object
				UIAction mActionCallback;

				//! Store the data pointer to pass to the action
				void* mActionData;
			};
		}
	}
}