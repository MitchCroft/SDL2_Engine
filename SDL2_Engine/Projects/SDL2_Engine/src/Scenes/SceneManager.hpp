#pragma once

//! Include the SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "../Utilities/IGlobal.hpp"
#include "../Utilities/TypeID.hpp"

namespace SDL2_Engine {
	//! Prototype the Scene Manager Initialiser object
	namespace Initialisation { struct SceneManagerInitialiser; }

	namespace Scenes {
		//! Prototype the ISceneBase object
		class ISceneBase;

		/*
		 *		Name: SceneManager
		 *		Author: Mitchell Croft
		 *		Created: 11/10/2017
		 *		Modified: 06/11/2017
		 *		
		 *		Purpose:
		 *		Provide an interface for controlling the updating and rendering of
		 *		'scenes' throughout the runtime of the program
		**/
		class SDL2_LIB_INC SceneManager : public Utilities::IGlobal {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////----------------------------------Management Functions-------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				SceneManager : addScene - Add a new Scene of a specified type with specified values
				Created: 11/10/2017
				Modified: 11/10/2017

				Template T - The type of Scene to create
				Template TArgs - A parameter pack of types used to setup the new Scene

				param[in] pArgs - A parameter pack of values used to setup the new Scene

				return bool - Returns true if the Scene was successfully created and introduced to the Manager
			*/
			template<typename T, typename ... TArgs>
			inline bool addScene(TArgs ... pArgs) {
				//Ensure the template is of the correct type
				static_assert(std::is_base_of<ISceneBase, T>::value, "Can not add a type that is not a subclass of ISceneBase as a new Scene in the Scene Manager");

				//Initialise the new Scene
				return initialiseScene(new T(pArgs...), Utilities::typeToID<T>());
			}

			/*
				SceneManager : retrieveScene - Retrieve the first active Scene of the specified type
				Created: 06/11/2017
				Modified: 06/11/2017

				Template T - The type of Scene to retrieve

				return T* - Returns a pointer to the first Scene of type T or nullptr if not found
			*/
			template<typename T>
			inline T* retrieveScene() {
				//Ensure the template is of the correct type
				static_assert(std::is_base_of<ISceneBase, T>::value, "Can not retrieve a type that is not a subclass of ISceneBase from the Scene Manager");

				//Find the Scene
				return (T*)retrieveScene(Utilities::typeToID<T>());
			}

			/*
				SceneManager : retrieveScene - Retrieve the first active Scene of the specified type
				Created: 06/11/2017
				Modified: 06/11/2017

				param[in] pID - The Type ID of the Scene to retrieve

				return ISceneBase* - Returns a pointer to the first active Scene of with a matching typeID or nullptr if not found
			*/
			ISceneBase* retrieveScene(const Utilities::typeID& pID);

			/*
				SceneManager : removeScene - Flag the first Scene of the specified type for removal
				Created: 11/10/2017
				Modified: 11/10/2017

				Template T - The type of Scene to remove

				return bool - Returns true if a Scene of the specified type was flagged
			*/
			template<typename T>
			inline bool removeScene() {
				//Ensure the template is of the correct type
				static_assert(std::is_base_of<ISceneBase, T>::value, "Can not remove a type that is not a subclass of ISceneBase from the Scene Manager");
				return removeScene(Utilities::typeToID<T>());
			}

			/*
				SceneManager : removeScene - Flag the first scene of the specified type for removal
				Created: 11/10/2017
				Modified: 11/10/2017

				param[in] pID - The Type ID of the Scene to remove

				return bool - Returns true if a Scene of the specified type was flagged
			*/
			bool removeScene(const Utilities::typeID& pID);

			/*
				SceneManager : removeScenes - Flag all scenes of the specified type for removal
				Created: 11/10/2017
				Modified: 11/10/2017

				Template T - The type of Scene to remove

				return bool - Returns true if a Scene of the specified type was flagged
			*/
			template<typename T>
			inline bool removeScenes() {
				//Ensure the template is of the correct type
				static_assert(std::is_base_of<ISceneBase, T>::value, "Can not remove a type that is not a subclass of ISceneBase from the Scene Manager");
				return removeScenes(Utilities::typeToID<T>());
			}

			/*
				SceneManager : removeScenes - Flag of scenes of the specified type for removal
				Created: 11/10/2017
				Modified: 11/10/2017

				param[in] pID - The Type ID of the Scene to remove

				return bool - Returns true if a Scene of the specified type was flagged
			*/
			bool removeScenes(const Utilities::typeID& pID);

			/*
				SceneManager : quit - Flag to the program that it should terminate
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			void quit();

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------------Data Accessors----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				SceneManager : isRunning - Get the running flag of the program
				Created: 11/10/2017
				Modified: 11/10/2017

				return const bool& - Returns a constant reference to the running flag
			*/
			const bool& isRunning() const;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Construction/Destruction-----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				SceneManager : Constructor - Initialise with default values
				Created: 11/10/2017
				Modified: 11/10/2017

				param[in] pSetup - Defines how the Scene Manager should be setup
			*/
			SceneManager(Initialisation::SceneManagerInitialiser* pSetup);

			/*
				SceneManager : createInterface - Verify and setup starting information
				Created: 11/10/2017
				Modified: 11/10/2017

				return bool - Returns true if the Resources Manager was setup correctly
			*/
			bool createInterface() override;

			/*
				SceneManager : destroyInterface - Deallocate internal memory allocated
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			void destroyInterface() override;

			/*
				SceneManager : update - Update and render the contained Scenes
				Created: 11/10/2017
				Modified: 02/11/2017
			*/
			void update() override;

		private:
			//! Define the internal protected elements for the Renderer
			struct SceneManagerInternalData;
			SceneManagerInternalData* mData;

			//! Initialise a new scene
			bool initialiseScene(ISceneBase* pScene, const Utilities::typeID& pID);
		};
	}
}