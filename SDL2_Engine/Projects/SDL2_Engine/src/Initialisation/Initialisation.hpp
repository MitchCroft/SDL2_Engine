#pragma once
#include "../__LibraryManagement.hpp"

//! Include the Initialisation objects
#include "../Debug/LoggerInitialiser.hpp"
#include "../Window/WindowInitialiser.hpp"
#include "../Rendering/RendererInitialiser.hpp"
#include "../Audio/AudioInitialiser.hpp"
#include "../Scenes/SceneManagerInitialiser.hpp"

namespace SDL2_Engine {
	namespace Initialisation {
		//! Label the errors that can occur during the initialisation phase
		enum class EInitialisationError {
			Success,
			Logger_Initialisation_Failed,
			Math_Initialisation_Failed,
			Window_Initialisation_Failed,
			Time_Initialisation_Failed,
			Controllers_Initialisation_Failed,
			Keyboard_Initialisation_Failed,
			Mouse_Initialisation_Failed,
			Rendering_Initialisation_Failed,
			Audio_Initialisation_Failed,
			Local_Resources_Initialisation_Failed,
			UI_Initialisation_Failed,
			Scene_Management_Initialisation_Failed,
		};

		//! Flag the Engine systems that are to be initialised
		enum class EInitialiseSystems : char {
			//! Global Time object, providing delta and runtime tracking functionality
			Time			= 1 << 0,

			//! Input from gamepads, keyboard and mouse
			Input			= (1 << 1) | Time,

			//! Rendering capabilities for images and text
			Rendering		= 1 << 2,

			//! Audio music and sound effect playback
			Audio			= 1 << 3,

			//! Handles the loading of local resources
			Local_Resources = 1 << 4,

			//! Manage users interaction with and displaying of elements
			UI				= 1 << 5 | Rendering | Input,

			//! Automatically handles the loading of all systems
			All				= Time | Input | Rendering | Audio | Local_Resources | UI
		};

		/*
		 *		Name: SDL2_Engine_Initialiser
		 *		Author: Mitchell Croft
		 *		Created: 06/10/2017
		 *		Modified: 06/10/2017
		 *		
		 *		Purpose:
		 *		Store the initialisation setup parameters 
		**/
		struct SDL2_LIB_INC SDL2_Engine_Initialiser {
			//! Indicate the engine systems that should be initialised
			Utilities::Bitmask<EInitialiseSystems> initialiseSystems = EInitialiseSystems::All;

			//! Store the values used to initialise the Logger object
			LoggerInitialiser loggerValues;

			//! Store the values used to initialise the Window object
			WindowInitialiser windowValues;

			//! Store the values used to initialise the Renderer object
			RendererInitialiser rendererValues;

			//! Store the values used to initialise the Audio object
			AudioInitialiser audioValues;

			//! Store the values used to initialise the Scene Manager object
			SceneManagerInitialiser* sceneManagerValues = nullptr;
		};

		/*
			SDL2_Engine_Init - Initialise the SDL2_Engine and begin operation
			Author: Mitchell Croft
			Created: 06/10/2017
			Modified: 11/10/2017

			param[in] pSetup - An SDL2_Engine_Initialiser object defining how the program should be created

			return EInitialisationError - Returns a EInitialisationError code indicating the success of the operation
		*/
		extern SDL2_LIB_INC EInitialisationError SDL2_Engine_Init(const SDL2_Engine_Initialiser& pSetup);
	}
}