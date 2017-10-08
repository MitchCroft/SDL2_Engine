#include <iostream>

#include <Globals.hpp>
#include <Time.hpp>
#include <Input/Keyboard/Keyboard.hpp>
#include <Input/Controllers/Controllers.hpp>
#include <Input/Controllers/VibrationDescription.hpp>
#include <Math.hpp>
#include <Debug/Logger.hpp>

#include <Resources/ResourceTypes/LocalResourceTexture.hpp>
#include <Resources/ResourceTypes/LocalResourceSFX.hpp>
#include <Resources/ResourceTypes/LocalResourceGeneric.hpp>
#include <Resources/Resources.hpp>

using namespace SDL2_Engine;
using namespace SDL2_Engine::Input;
using namespace SDL2_Engine::Debug;

float scaleFunc(const float& pProg) { return 1.f - pProg; }

int main() {
	//Create the global interface
	Globals::create();

	//Create the global Time object
	if (Globals::addInterface<Logger>() && Globals::addInterface<Time>() && Globals::addInterface<Math>() && Globals::addInterface<Keyboard>() && Globals::addInterface<Resources>(nullptr)) {
		//Get references to all of the Global elements
		Time& time = Globals::get<Time>();
		Keyboard& keyboard = Globals::get<Keyboard>();
		Math& math = Globals::get<Math>();
		Logger& logger = Globals::get<Logger>();
		Resources& resources = Globals::get<Resources>();

		//Enable the logger
		//logger.setLoggingOutput(true);
		
		auto file = resources.loadResource<Generic>("test.txt");

		//Imitate a game loop for testing purposes
		while (!keyboard.keyPressed(EKeyboardKeyCodes::Escape)) {
			//Update the Globals
			Globals::update();
		}
	}

	//Otherwise output error message
	else printf("Failed to create the Global Interfaces\n");

	//Destroy the Global objects
	Globals::destroy();
	return 0;
}