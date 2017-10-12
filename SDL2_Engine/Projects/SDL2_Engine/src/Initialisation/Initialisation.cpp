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
#include "../Audio/Audio.hpp"
#include "../Resources/Resources.hpp"
//TODO: Add UI management
#include "../Scenes/SceneManager.hpp"

//! Include the SDL functions
#include <SDL.h>

namespace SDL2_Engine {
	namespace Initialisation {
		/*
			SDL2_Engine_Init - Initialise the SDL2_Engine and begin operation
			Author: Mitchell Croft
			Created: 06/10/2017
			Modified: 12/10/2017

			param[in] pSetup - An SDL2_Engine_Initialiser object defining how the program should be created

			return EInitialisationError - Returns a EInitialisationError code indicating the success of the operation
		*/
		EInitialisationError SDL2_Engine_Init(const SDL2_Engine_Initialiser& pSetup) {
			//Create the Globals interface
			Globals::create();

			//Store the error return
			EInitialisationError errorNum = EInitialisationError::Success;

			//Store a reference to the required Global objects
			Window* window = nullptr;
			Rendering::Renderer* renderer = nullptr;
			Scenes::SceneManager* sceneManager = nullptr;

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
						if (!(renderer = Globals::addInterface<Rendering::Renderer>(window->getWindow(), pSetup.rendererValues))) errorNum = EInitialisationError::Rendering_Initialisation_Failed;

					//Check for Audio
					if (!(int)errorNum && pSetup.initialiseSystems & EInitialiseSystems::Audio)
						if (!Globals::addInterface<Audio>(pSetup.audioValues)) errorNum = EInitialisationError::Audio_Initialisation_Failed;

					//Check for Local Resources
					if (!(int)errorNum && pSetup.initialiseSystems & EInitialiseSystems::Local_Resources) 
						if (!Globals::addInterface<Resources>(renderer->getRenderer())) errorNum = EInitialisationError::Local_Resources_Initialisation_Failed;

					//Setup the Scene Manager
					if (!(int)errorNum)
						if (!(sceneManager = Globals::addInterface<Scenes::SceneManager>(pSetup.sceneManagerValues))) errorNum = EInitialisationError::Scene_Management_Initialisation_Failed;

					//Check for UI
					if (!(int)errorNum && pSetup.initialiseSystems & EInitialiseSystems::UI) {
						//TODO
					}
					
					//Run the Game Loop
					if (!(int)errorNum) {
						do {
							//Update SDL events
							SDL_PumpEvents();

							//Update the global objects
							Globals::update();

							//Render the frame
							renderer->presentFrame();
						} while (sceneManager->isRunning());
					}
				}

				//Math failed to create
				else errorNum = EInitialisationError::Math_Initialisation_Failed;
			}

			//Logger failed to create
			else errorNum = EInitialisationError::Logger_Initialisation_Failed;

			//Destroy the Globals interfaces
			Globals::destroy();

			//Close down SDL
			SDL_Quit();

			//Return the error code
			return errorNum;
		}
	}
}