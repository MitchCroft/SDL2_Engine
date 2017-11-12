#pragma once

//! Include the SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "../Utilities/Bitmask.hpp"
#include "ScenesValues.hpp"

namespace SDL2_Engine {
	namespace Scenes {
		//! Define a type for rendering order
		typedef unsigned char renderOrder;

		//! Export the required Bitmask implementation
		template class SDL2_LIB_INC Utilities::Bitmask<ESceneProperties>;

		/*
		 *		Name: ISceneBase
		 *		Author: Mitchell Croft
		 *		Created: 11/10/2017
		 *		Modified: 11/10/2017
		 *		
		 *		Purpose:
		 *		Provide an interface for Base Scenes to implement from
		**/
		class SDL2_LIB_INC ISceneBase {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------Implementable Interface-------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				ISceneBase : Constructor - Initialise with default values
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			ISceneBase();

			/*
				ISceneBase : Destructor - Override-able destructor to allow for memory cleanup
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			virtual ~ISceneBase() {}

			/*
				ISceneBase : createScene - Function to facilitate the setup and validation of a Scene
				Created: 11/10/2017
				Modified: 11/10/2017

				return bool - Returns true if the Scene was created successfully
			*/
			virtual bool createScene() = 0;

			/*
				ISceneBase : destroyScene - Function to facilitate the deallocation of resources for a Scene
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			virtual void destroyScene() = 0;

			/*
				ISceneBase : update - Function to facilitate the updating of values once per cycle
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			inline virtual void update() {}

			/*
				ISceneBase : render - Function to facilitate the rendering of images once per cycle
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			inline virtual void render() {}

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------------Data Accessors------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				ISceneBase : getRenderOrder - Retrieve the render order value for the Scene
				Created: 11/10/2017
				Modified: 11/10/2017

				return const renderOrder& - Returns a constant reference to the render order value
			*/
			inline const renderOrder& getRenderOrder() const { return mRenderOrder; }

			/*
				ISceneBase : getProperties - Retrieve the properties value for the Scene
				Created: 11/10/2017
				Modified: 11/10/2017

				return const Utilities::Bitmask<ESceneProperties>& - Returns a constant reference to the properties bitmask
			*/
			inline const Utilities::Bitmask<ESceneProperties>& getProperties() const { return mProperties; }

			/*
				ISceneBase : getTypeID - Retrieve the type ID of the Scene
				Created: 11/10/2017
				Modified: 11/10/2017

				return const size_t& - Returns a constant reference to the type ID value
			*/
			inline const size_t& getTypeID() const { return mTypeID; }

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------Property Modifiers----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				ISceneBase : shutdown - Flag the Scene for shutdown
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			void shutdown();

			/*
				ISceneBase : show - Flag the Scene to be rendered
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			void show();

			/*
				ISceneBase : hide - Flag the Scene to be hidden
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			void hide();

			/*
				ISceneBase : prioritise - Flag the scene to take priority
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			void prioritise();

			/*
				ISceneBase : deprioritise - Flag the scene to remove priority
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			void deprioritise();

		protected:
			//! Store the rendering order, 0 being the foreground and 255 being the background
			renderOrder mRenderOrder;

		private:
			//! Set as a friend of the Scene Manager
			friend class SceneManager;

			//! Store the current state of the Scene
			Utilities::Bitmask<ESceneProperties> mProperties;

			//! Store a type-specific unique identifying ID value
			size_t mTypeID;
		};
	}
}
