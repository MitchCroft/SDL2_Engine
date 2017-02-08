#pragma once

#include "../Types.hpp"
#include "ManagementValues.hpp"

//! Include the Read Only properties 
#include "../Utilities/Properties/ReadOnlyProperty.hpp"

namespace SDL2_Engine {
	//! Prototype the Time object
	class Time;

	namespace ScreenManagement {
		/*
		 *		Name: BaseScreen
		 *		Author: Mitchell Croft
		 *		Created: 06/02/2017
		 *		Modified: 06/02/2017
		 *		
		 *		Purpose:
		 *		Provide a base point for game screens to be 
		 *		created from so that they can be used with
		 *		Scene Manager
		**/
		class BaseScreen {
			//! Make a friend of the SceneManager (Setting up the ID values)
			friend class SceneManager;

			//! Store the ID of the current Screen object
			uint mID;

		protected:
			//! Store the status of the current Screen
			EScreenState mStatus;

			//! Store the 'Z' distance of the Screen 
			ubyte mExecutionOrder;

		public:
			//! Give read only property access to external objects
			Utilities::Properties::ReadOnlyProperty<uint> id;
			Utilities::Properties::ReadOnlyProperty<EScreenState> status;
			Utilities::Properties::ReadOnlyProperty<ubyte> executionOrder;

			//! Constructor / Destructor
			BaseScreen() : mID(0), mStatus(EScreenState::Active), mExecutionOrder(0), id(mID), status(mStatus), executionOrder(mExecutionOrder) {}
			inline virtual ~BaseScreen() {}

			//! Main functions
			virtual bool startup() = 0;
			inline virtual void update(const Time& pTime) {}
			inline virtual void handleInput(const Time& pTime) {}
			inline virtual void draw(/* TODO: Add Rendering Pointer */) const {}
			inline virtual void shutdown() {}

			//! Separate thread function
			inline virtual void threadedUpdate(const Time& pTime) {}
		};
	}
}