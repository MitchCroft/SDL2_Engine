#include "Initialisation.hpp"

//! Include all of the SDL2_Engine headers required for initialisation
#include "../Globals.hpp"

#include "../Debug/Logger.hpp"
#include "../Math.hpp"
#include "../Window/Window.hpp"
#include "../Time.hpp"
#include "../Input/Controllers/Controllers.hpp"
#include "../Input/Keyboard/Keyboard.hpp"
#include "../Input/Mouse/Mouse.hpp"
#include "../Rendering/Renderer.hpp"
//TODO: Add audio
#include "../Resources/Resources.hpp"
//TODO: Add scene management

//! Include the SDL functions
#include <SDL.h>

namespace SDL2_Engine {
	namespace Initialisation {
		/*
			SDL2_Engine_Init - Initialise the SDL2_Engine and begin operation
			Author: Mitchell Croft
			Created: 06/10/2017
			Modified: 06/10/2017

			param[in] pSetup - An SDL2_Engine_Initialiser object defining how the program should be created

			return EInitialisationError - Returns a EInitialisationError code indicating the success of the operation
		*/
		EInitialisationError SDL2_Engine_Init(const SDL2_Engine_Initialiser& pSetup) {
			//Create the Globals interface
			Globals::create();

			//Store the error return
			EInitialisationError errorNum = EInitialisationError::Success;

			//Store a reference to the Window and Scene Manager
			Window* window = nullptr;

			//Create the Logger object
			if (Globals::addInterface<Debug::Logger>(pSetup.loggerValues)) {
				//Create the Math object
				if (Globals::addInterface<Math>()) {
					//Prompt initial SDL setup
					SDL_Init(NULL);

					/////////////////////////////////////////////////////////////////////////////////////////////////////
					////////-------------------------------------Setup Systems-----------------------------------////////
					/////////////////////////////////////////////////////////////////////////////////////////////////////
					
					//Setup the window
					if (!(window = Globals::addInterface<Window>(pSetup.windowValues))) errorNum = EInitialisationError::Window_Initialisation_Failed;

					//Check for Time inclusion
					if (!(int)errorNum && pSetup.initialiseSystems & EInitialiseSystems::Time)
						if (!Globals::addInterface<Time>()) errorNum = EInitialisationError::Time_Initialisation_Failed;
					
					//Check for Input
					if (!(int)errorNum && pSetup.initialiseSystems & EInitialiseSystems::Input) {
						//Create the Controllers object
						if (!Globals::addInterface<Input::Controllers>()) errorNum = EInitialisationError::Controllers_Initialisation_Failed;

						//Create the Keyboard object
						else if (!Globals::addInterface<Input::Keyboard>()) errorNum = EInitialisationError::Keyboard_Initialisation_Failed;

						//Create the Mouse object
						else if (!Globals::addInterface<Input::Mouse>()) errorNum = EInitialisationError::Mouse_Initialisation_Failed;
					}
					
					//Check for Rendering
					if (!(int)errorNum && pSetup.initialiseSystems & EInitialiseSystems::Rendering) 
						if (!Globals::addInterface<Rendering::Renderer>(window->getWindow(), pSetup.rendererValues)) errorNum = EInitialisationError::Rendering_Initialisation_Failed;

					//Check for Audio
					if (!(int)errorNum && pSetup.initialiseSystems & EInitialiseSystems::Audio) {
						//TODO
					}

					//Check for Local Resources
					if (!(int)errorNum && pSetup.initialiseSystems & EInitialiseSystems::Local_Resources) 
						if (!Globals::addInterface<Resources>()) errorNum = EInitialisationError::Local_Resources_Initialisation_Failed;

					//Setup the Scene Manager
					//TODO
				}

				//Math failed to create
				else errorNum = EInitialisationError::Math_Initialisation_Failed;
			}

			//Logger failed to create
			else errorNum = EInitialisationError::Logger_Initialisation_Failed;

			//Run the Game Loop
			//TODO

			//Destroy the Globals interfaces
			Globals::destroy();

			//Close down SDL
			SDL_Quit();

			//Return the error code
			return errorNum;
		}
	}
}