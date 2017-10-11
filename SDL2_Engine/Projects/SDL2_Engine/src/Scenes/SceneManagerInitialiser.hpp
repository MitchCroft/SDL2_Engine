#pragma once

//! Include SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "../Utilities/TypeID.hpp"

namespace SDL2_Engine {
	//! Prototype the ISceneBase object
	namespace Scenes { class ISceneBase; }

	namespace Initialisation {
		/*
		 *		Name: SceneManagerInitialiser
		 *		Author: Mitchell Croft
		 *		Created: 11/10/2017
		 *		Modified: 11/10/2017
		 *		
		 *		Purpose:
		 *		Store initialisation values for the Scene Manager object
		**/
		struct SceneManagerInitialiser {
			//! Store a pointer to the initial screen to use in the program
			Scenes::ISceneBase* const initialScreen;

			//! Store the type ID of the initial screen
			const Utilities::typeID initialID;

			/*
				SceneManagerInitialiser : Constructor - Initialise with default values
				Created: 11/10/2017
				Modified: 11/10/2017

				Template T - The type of Scene to initialise with
			*/
			template<typename T>
			inline SceneManagerInitialiser(T* pInitial) : initialScreen(pInitial), initialID(Utilities::typeToID<T>()) {
				//Ensure the template is of the correct type
				static_assert(std::is_base_of<Scenes::ISceneBase, T>::value, "Can not setup the SceneManagerInitialiser as a type that is not a subclass of ISceneBase");
			}
		};
	}
}